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

#ifndef STREAMLINK_FILEINTERFACE_H
#define STREAMLINK_FILEINTERFACE_H
#include <streamfs/VirtualFSProvider.h>

class VirtualFSProvider;

class FileInterface {
public:
    virtual void registerFsProvider(VirtualFSProvider* provider) = 0 ;
    virtual void removeFsProvider(std::string id)  = 0;
};

#endif //STREAMLINK_FILEINTERFACE_H
