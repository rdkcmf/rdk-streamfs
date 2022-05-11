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

#ifndef STREAMLINK_MESSAGEQUEUE_H
#define STREAMLINK_MESSAGEQUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>

/**
 * Generic message queue
 * @param T - message type
 */
template <class T>
class MessageQueue {

public:
    T waitForMessage() {
        std::unique_lock<std::mutex> lock(mMtx);
        T res;
        this->mCond.wait(lock, [=]{ return !mQueue.empty(); });
        res = mQueue.front();
        mQueue.pop();
        return res;
    }

    void pushMessage(T msg) {
        std::unique_lock<std::mutex> lock(mMtx);
        mQueue.push(msg);
        this->mCond.notify_one();
    }

    int getMessage(T &msg) {
        std::lock_guard<std::mutex> lock(mMtx);
        if(mQueue.empty()) {
            return 0;
        }
        msg = mQueue.front();
        mQueue.pop();
        return -1;
    }

private:
    std::condition_variable mCond;
    std::mutex mMtx;
    std::queue<T> mQueue;
};

#endif //STREAMLINK_MESSAGEQUEUE_H
