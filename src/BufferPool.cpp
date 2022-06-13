/*
 * If not stated otherwise in this file or this component's LICENSE
 * file the following copyright and licenses apply:
 *
 * Copyright (c) 2022 Nuuday.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <glog/logging.h>
#include "streamfs/BufferPool.h"
#include <cstring>

#include <boost/thread/locks.hpp>
#include <condition_variable>
#include "streamfs/BufferPoolThrottle.h"

template class BufferPool<buffer_chunk>;

template <typename T>
BufferPool<T>::BufferPool (
        std::shared_ptr<BufferProducer<T>> producer,
        std::shared_ptr<BufferConsumer<T>> consumer,
        uint64_t preAllocBufSize)
        : mLastReadLocation(0)
        , mReadEnd(0)
        , mProducer(producer)
        , mConsumer(consumer)
        , mCircBuf(preAllocBufSize)
        , mTotalBufCount(0)
        , mGotLastBuffer(false)
        , mLastBufferSize(0) {
    mProducer->setBufferPool(this);
#ifdef BUFFER_CHUNK_READ_THROTTLING
    mThrottle = std::make_unique<BufferPoolThrottle>(preAllocBufSize);
#endif
}

template <typename T>
BufferPool<T>::~BufferPool() {
    mProducer->stop();
    exitPending = true;
    {
        boost::mutex::scoped_lock lock(m_w_mutex);
    }
}

template <typename T>
void BufferPool<T>::enableReadThrottling(bool enable) {
#ifdef BUFFER_CHUNK_READ_THROTTLING
    mThrottle->enableThrottling(enable);
#endif
}

template<>
size_t BufferPool<buffer_chunk>::read(
        char *bufferChunks,
        size_t length,
        uint64_t offset,
        size_t left_padding,
        size_t right_padding) {

    if (exitPending) {
        LOG(WARNING) << "BufferPool::read : return 0 : exitPending";
        return 0;
    }

    boost::mutex::scoped_lock lock(m_mutex);

    auto capacity = mCircBuf.capacity();

    if ((mGotLastBuffer && offset >= mTotalBufCount)) {
        // producer closed the data stream.
        LOG(WARNING) << "BufferPool::read : return 0 : offset : " << offset << " mTotalBufCount : " << mTotalBufCount;
        return 0;
    }

    if (left_padding >= BUFFER_CHUNK_SIZE ||
        right_padding >= BUFFER_CHUNK_SIZE ||
        (length - left_padding - right_padding) < 0) {
        LOG(WARNING) << "BufferPool::read : return 0 : length : " << length << " left_padding : " << left_padding<< " right_padding : " << right_padding;
        return 0;
    }

    if ((mTotalBufCount - offset) > mCircBuf.capacity()
        || offset > mTotalBufCount
        || length > capacity
            ) {
        LOG(WARNING) << "BufferPool::read : return 0 : length : " <<
            length << " buffer-offset : " << offset << " mTotalBufCount : " <<
            mTotalBufCount << " mCircBuf.capacity() : " << mCircBuf.capacity()
            << " capacity : " << capacity;
        return 0;
    }

    auto readEnd = offset + length;
    if (readEnd > mTotalBufCount && !mGotLastBuffer) {
        boost::chrono::duration<double> timeout = boost::chrono::milliseconds(BUFFER_POOL_READ_TIMEOUT_MS);

        if (!mNotEnoughBytes.wait_for(lock, timeout, [this, readEnd](){
            auto result = readEnd <= mTotalBufCount || mGotLastBuffer || mAborting;
            return result;
          })) {
            LOG(INFO) << __FUNCTION__ << ": no buffer chunks received before read timout";
            return 0;
        }

        if (mAborting) {
            LOG(INFO) << __FUNCTION__ << ": Aborting read";
            mAborting = false;
            return 0;
        }

        mReadEnd = readEnd;
    }


    auto off = 0;
    {
        boost::mutex::scoped_lock lock(m_w_mutex);

        //RDK-2310 workaround
        if ( mCircBuf.empty()) {
            LOG(INFO) << "RDK-2310 BUG: size should be non-zero. exit read";
            return 0;
        }

        size_t lastItemCopySize;
        size_t leftSkip;
        size_t rightSkip;

        for (size_t i = 0; i < length; i++) {

            if (i == 0) {
                leftSkip = left_padding;
            } else {
                leftSkip = 0;
            }

            if (i == length - 1) {
                rightSkip = right_padding;
            } else {
                rightSkip = 0;
            }

            lastItemCopySize = BUFFER_CHUNK_SIZE - leftSkip - rightSkip;

            if (mGotLastBuffer && i == mTotalBufCount - 1) {
                lastItemCopySize = std::min(mLastBufferSize, lastItemCopySize);
            }

            auto endPos = std::min(mCircBuf.size(), capacity);
            auto startPos = endPos - (mTotalBufCount - offset) + i;

            mLastReadLocation = startPos;
#ifdef BUFFER_CHUNK_READ_THROTTLING
            mThrottle->setReadPosition(startPos);
#endif
            memcpy(bufferChunks + off,
                   mCircBuf[startPos].data() + leftSkip,
                   lastItemCopySize);
            off += lastItemCopySize;
        }
    }

#ifdef BUFFER_CHUNK_READ_THROTTLING
    mThrottle->wait();
#endif
    return off;
}

template<>
size_t BufferPool<buffer_chunk>::readRandomAccess(char *data, size_t size, uint64_t offsetBytes) {
    auto left_padding = offsetBytes % BUFFER_CHUNK_SIZE;
    auto right_padding = (BUFFER_CHUNK_SIZE - (left_padding + size) % BUFFER_CHUNK_SIZE) % BUFFER_CHUNK_SIZE;
    size_t length = (left_padding + right_padding + size) / BUFFER_CHUNK_SIZE;
    auto offset = offsetBytes / BUFFER_CHUNK_SIZE;
    return read(data, length, offset, left_padding, right_padding);
}

template<>
void BufferPool<buffer_chunk>::clear() {
    boost::mutex::scoped_lock lock(m_w_mutex);
    mGotLastBuffer = false;
    mTotalBufCount = 0;
    mReadEnd = 0;
    mLastBufferSize = 0;
    mCircBuf.clear();
#ifdef BUFFER_CHUNK_READ_THROTTLING
    mThrottle->clear();
#endif
}

template<>
void BufferPool<buffer_chunk>::pushBuffer(const buffer_chunk &buffer, bool lastBuffer, size_t lastBufferSize) {

    if (exitPending)
        return;
#ifdef BUFFER_CHUNK_READ_THROTTLING
    mThrottle->registerTimePeriod();
#endif
    boost::mutex::scoped_lock lock(m_w_mutex);
    {
        mCircBuf.push_back(buffer);
        mTotalBufCount++;
        if (lastBuffer) {
            mGotLastBuffer = true;
            mLastBufferSize = lastBufferSize;
        }
        mNotEnoughBytes.notify_one();
    }

    mConsumer->newBufferAvailable(mCircBuf.front());
}

template<typename T>
void BufferPool<T>::lockWaitForRead() {

}

template<>
void BufferPool<buffer_chunk>::clearToLastRead() {
    boost::mutex::scoped_lock lock(m_mutex);
    boost::mutex::scoped_lock lockWrite(m_w_mutex);

    if (mCircBuf.empty())
        return;

    auto numElements = mCircBuf.size();

    if (numElements > mLastReadLocation) {
        auto eraseCount = numElements - mLastReadLocation - 1;
        mCircBuf.erase_end(eraseCount);
        mTotalBufCount -= eraseCount;
    }
    mAborting = false;

#ifdef BUFFER_CHUNK_READ_THROTTLING
    mThrottle->clearToLastRead();
#endif
}

template<>
void BufferPool<buffer_chunk>::abortAllOperations() {
    mAborting = true ;
    mNotEnoughBytes.notify_all();
    boost::mutex::scoped_lock lock(m_w_mutex);
    mProducer->stop();
}
