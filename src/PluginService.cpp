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

#include <thread>
#include <glog/logging.h>
#include "streamfs/PluginService.h"

namespace streamfs {

void PluginService::start() {
    if (mThread != nullptr) {
        LOG(ERROR) << "Plugin service already started";
        return;
    }

    mThread = new std::thread(&PluginService::loop, this);
}

void PluginService::loop() {
    ServiceRequest msg;
    while (!pendingExit()) {
        mServReq.getMessage(msg);
        switch (msg.type) {
            case ServiceRequestEnum::START:
                break;
            case ServiceRequestEnum::STOP:
                stopPlayback();
            default:
                LOG(WARNING) << "Service message type " << msg.type << " not in use";
        }
    }
}

bool PluginService::pendingExit() {
    return false;
}

}//namespace streamfs;
