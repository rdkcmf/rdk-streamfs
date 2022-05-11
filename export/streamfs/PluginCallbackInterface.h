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
#include <vector>
#include <string>
#include "PluginConfig.h"
namespace streamfs {
class PluginCallbackInterface {
public:
    virtual void setAvailableStreams(const std::vector<std::string> &streamIds) = 0;

    virtual void updateConfig(const PluginConfig &config) = 0;

    virtual const std::vector<std::string> & getAvailableStreams() = 0;

    /**
     * Notify file changed (used in poll)
     * @param path
     */
    virtual void notifyUpdate(const std::string& path) = 0;

    virtual ~PluginCallbackInterface() = default;
};
}
