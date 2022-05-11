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

#include <boost/thread/condition.hpp>

/*
 * Helper class wrapping condition_variable to prevent:
 *
 * - lost wake-up (i.e. race condition when notify is called before wait)
 * - spurious wakeup, which may happen that the receiver wakes up, although
 *   no notification happened. POSIX Threads can be a victim of this phenomena.
 *
 * NOTE: this class only supports one wait across all thread, since there is only
 * one state variable (mNotified) related to managing this.
 */
class ConditionVariableHelper {
public:

    ConditionVariableHelper() : mNotified(false) {};

    void notifyOne() {
        {
            boost::lock_guard<boost::mutex> lock(mMutex);
            mNotified = true;
        }
        mConditionVariable.notify_one();
    }

    void wait() {
        boost::unique_lock<boost::mutex> lock(mMutex);
        mConditionVariable.wait(lock,[this](){ return mNotified; });
        mNotified = false;
    }

    template<typename predicate_type>
    void wait(predicate_type pred) {
        boost::unique_lock<boost::mutex> lock(mMutex);
        mConditionVariable.wait(lock,[&pred, this](){ bool status = pred(); return mNotified || status; });
        mNotified = false;
    }

    template<typename predicate_type>
    bool wait_for(uint64_t timeout_ms, predicate_type pred) {
        boost::unique_lock<boost::mutex> lock(mMutex);
        boost::chrono::duration<double> timeout = boost::chrono::milliseconds(timeout_ms);
        bool status = mConditionVariable.wait_for(lock, timeout, [&pred, this](){bool status = pred(); return mNotified || status; });
        mNotified = false;
        return status;
    }

private:
    bool mNotified;
    boost::mutex mMutex;
    boost::condition_variable mConditionVariable;
};
