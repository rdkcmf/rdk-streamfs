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

#include <streamfs/PluginInterface.h>
#include <memory>
#include <string>
#include <utility>
#include <streamfs/BufferConsumer.h>
#include <streamfs/ByteBufferPool.h>
#include "streamfs/PluginCallbackInterface.h"
#include "fuse/IFuse.h"

namespace streamfs {

class SamplePlugin : public PluginInterface {
    typedef std::string uri_type;
    std::map<uri_type, std::shared_ptr<ByteBufferPool>> mBufferPool;

public:
    explicit SamplePlugin(PluginCallbackInterface* cb,  debug_options_t *debugOptions);

    std::string getId() override;

    int open(std::string uri) override;

    void stopPlayback() override;

    void updateConfiguration(const PluginConfig &config) override;

    void newBufferNotify(buffer_chunk &buffer);

    int read(uint64_t handle, std::string path, char *buf, size_t size, uint64_t offset) override;

    int write(std::string node, const char *buf, size_t size, uint64_t offset) override;

    int release(uint64_t handle, std::string path) override;

    uint64_t getSize(std::string path) override;

private:
    std::weak_ptr<streamfs::PluginCallbackInterface> mCb;
    PluginConfig mConfig{};
    std::vector<std::string> mAvailableStreams;
    ByteBufferPool::shared_consumer_type mConsumer;
    std::mutex mFopMutex;
};

}

extern "C" {
    streamfs::PluginInterface *INIT_STREAMFS_PLUGIN(streamfs::PluginCallbackInterface *cb, debug_options_t *debugOptions) {
        return new streamfs::SamplePlugin(cb, debugOptions);
}

const char *GET_STREAMFS_PLUGIN_ID() {
    return "sample_plugin";
}

void UNLOAD_STREAMFS_PLUGIN(streamfs::PluginInterface *mSample) {
}

}
