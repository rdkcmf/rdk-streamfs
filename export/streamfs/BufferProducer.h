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

#pragma once

#include <cstddef>
#include <boost/thread/mutex.hpp>

template <typename T> class BufferPool;

template <class T>
class BufferProducer {
    template <typename T1>
    friend  class BufferPool;

public:
    virtual void queueBuffer(const T &buffer, bool lastBuffer = false, size_t lastBufferSize = 0) {
        mBufferPool->pushBuffer(buffer, lastBuffer, lastBufferSize);
    }

    virtual void stop();

    virtual uint64_t getTotalBufferCount() {
        boost::mutex::scoped_lock lock(m_mutex);
        return mBufferPool->getTotalBufferCount();
    }

    virtual ~BufferProducer() {}

protected:
    void setBufferPool(BufferPool<T> *bufferPool) {
        boost::mutex::scoped_lock lock(m_mutex);
        mBufferPool = bufferPool;
    }

private:
    BufferPool<T> *mBufferPool;
    boost::mutex m_mutex;
};
