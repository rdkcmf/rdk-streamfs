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

#ifndef STREAMLINK_VIRTUALFSPROVIDER_H
#define STREAMLINK_VIRTUALFSPROVIDER_H

#include <string>
#include <vector>
#include <set>
#include <memory>
#include <glog/logging.h>
#include <mutex>
#include <streamfs/FileInterface.h>
#include "streamfs/PluginInterface.h"
#include "PluginCbImpl.h"

enum NodeTypes {
    FILE_TYPE,
    DIRECTORY_TYPE,
};


class VirtualFSProvider {
    typedef std::string fileId;
    typedef std::set<fileId> fileList;

    struct FileNode {
        std::string name;
        VirtualFSProvider *provider;
        NodeTypes type;
    };

public:
    VirtualFSProvider(const std::string &name,
                      std::weak_ptr<streamfs::PluginInterface> cb,
                      FileInterface &fileInterface, bool isPlugin);

    ~VirtualFSProvider();

    std::string getName() { return mName; }


    int attachProvider(VirtualFSProvider *provider) {
        if (provider == nullptr)
            return -1;

        auto inf = mCb.lock();

        if (!inf)
            return -1;

        auto fileList = inf->getAvailableStreams();

        if (std::find(fileList.begin(), fileList.end(), provider->getName()) != fileList.end()) {
            LOG(ERROR) << "Can't attach new directory. File already exists:" << provider->getName();
            return -2;
        }

        mSubProviders.insert(std::unique_ptr<VirtualFSProvider>(provider));
        return 0;
    }

    void notifyUpdate(const std::string &module, const std::string &path);

    std::vector<FileNode> getNodes();

    int open(std::string node);

    int read(uint64_t handle, std::string node, char *buf, size_t size, uint64_t offset);

    int write(std::string node, const char *buf, size_t size, uint64_t offset);

    int release(uint64_t handle, const std::string &nodeName);

    uint64_t getSize(const std::string& nodeName);

private:
    std::weak_ptr<streamfs::PluginInterface> mCb;
    std::mutex mFileListLock;
    std::string mName;
    std::set<std::unique_ptr<VirtualFSProvider>> mSubProviders;
    FileInterface &mFileInteface;
    bool mIsPluginHandler;
};

#endif //STREAMLINK_VIRTUALFSPROVIDER_H
