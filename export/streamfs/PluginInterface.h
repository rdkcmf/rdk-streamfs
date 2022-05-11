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

#ifndef STREAMFS_PLUGININTERFACE_H
#define STREAMFS_PLUGININTERFACE_H

#include <atomic>
#include <string>
#include <vector>
#include <memory>
#include "PluginService.h"
#include "BufferProducer.h"
#include "BufferPool.h"
#include "PluginConfig.h"
#include "PluginService.h"
#include "PluginCallbackInterface.h"
#include "DebugOptions.h"

namespace streamfs {
class PluginService;

#define STREAMFS_INTERFACE_VERSION 4

/**
 * Generic plugin interface for 3rd party plugins
 */
class PluginInterface : public PluginService {
public:
    PluginInterface(PluginCallbackInterface* cb, debug_options_t* debugOptions) : PluginService(cb), mCb(cb),
    mDebugOptions(debugOptions) {};

    /**
   * Update plugin configuration. Called first time before `registerCallback`
   * @param config - configuration
   */
    virtual void updateConfiguration(const PluginConfig &config) = 0;

    /**
     * Get plugin identifier
     * @return
     */
    virtual std::string getId() = 0;

    /**
     * Stop play
     */
    virtual void stopPlayback() = 0;

    virtual int open(std::string path)  =  0;


    virtual int read(uint64_t handle, std::string path, char *buf, size_t size, uint64_t offset) =  0;

    virtual int release(uint64_t handle, std::string path) =  0;

    virtual uint64_t getSize(std::string path) =  0;

    virtual int write(std::string node,
                      const char *buf,
                      size_t size,
                      uint64_t offset) = 0;

    /**
     *
     * @return interfaces version
     */
    virtual uint32_t getInterfaceVersion() { return STREAMFS_INTERFACE_VERSION; }

    PluginCallbackInterface* mCb;

    debug_options_t * mDebugOptions;
};

}

extern "C" {
// Implement function in plugin to create a new plugin interface
streamfs::PluginInterface *INIT_STREAMFS_PLUGIN(streamfs::PluginCallbackInterface* cb,  debug_options_t *debugOptions);

// Implement function to return streamfs ID
const char *GET_STREAMFS_PLUGIN_ID();

// Implement function to unload streamfs plugins
void UNLOAD_STREAMFS_PLUGIN(streamfs::PluginInterface *mPluginsInt);

}
#endif //STREAMFS_PLUGININTERFACE_H
