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

#ifndef STREAMLINK_PLUGINCBIMPL_H
#define STREAMLINK_PLUGINCBIMPL_H

#include <vector>
#include <mutex>
#include "streamfs/PluginConfig.h"
#include "streamfs/PluginCallbackInterface.h"
#include <streamfs/VirtualFSProvider.h>

namespace streamfs {


class PluginCbImpl : public PluginCallbackInterface {
public:

    PluginCbImpl(std::string id) {
        mId = id;
    }

    void setAvailableStreams(const std::vector<std::string> &streamIds) override;

    void updateConfig(const PluginConfig &config) override;

    const std::vector<std::string> &getAvailableStreams() override;

    void notifyUpdate(const std::string &path) override;

    void registerCallbackHandler(VirtualFSProvider *fsProvider) {
        mCBHandler = fsProvider;
    }

private:
    std::mutex guard;
    std::vector<std::string> mStreamIds;
    std::string mId;
    VirtualFSProvider *mCBHandler;
};

}
#endif //STREAMLINK_PLUGINCBIMPL_H
