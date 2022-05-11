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

#ifndef STREAMLINK_PLUGINSERVICE_H
#define STREAMLINK_PLUGINSERVICE_H

#include <thread>
#include "MessageQueue.h"
#include "ServiceRequest.h"
#include "PluginConfig.h"
#include "PluginCallbackInterface.h"
class VirtualFSProvider;

namespace streamfs {

class PluginService {
public:
    PluginService(PluginCallbackInterface* cb) :mCb(cb){}
    void start();

    /**
     * Update plugin configuration
     * (called by plugin)
     */
    void updateConfig(PluginConfig &config) {
        mCb->updateConfig(config);
    }

    void setAvailableStreams(const std::vector<std::string>& streamIds)
    {
        mCb->setAvailableStreams(streamIds);
    }

    const std::vector<std::string> & getAvailableStreams() {
        auto& res = mCb->getAvailableStreams();
        return res;
    }

    /**
    * Get plugin identifier
    * @return
    */
    virtual std::string getId() = 0;

    /**
     * Stop play
     */
    virtual void stopPlayback() = 0;

    virtual int read(uint64_t handle, std::string path, char *buf, size_t size, uint64_t offset) =  0;

    virtual int open(std::string path)  =  0;

private:
    void loop();
    bool pendingExit();
private:
    std::thread *mThread{};
    MessageQueue<ServiceRequest> mServReq;
    PluginCallbackInterface* mCb;
};

}//namespace streamfs
#endif //STREAMLINK_PLUGINSERVICE_H
