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

#ifndef STREAMLINK_SERVICEREQUEST_H
#define STREAMLINK_SERVICEREQUEST_H


#include <stdint.h>
#include <string>

enum ServiceRequestEnum {
    START,         // start playback. Caller must supply an uri
    STOP,          // on stop player should release all resources.
    PAUSE,         // Pause playback
    RELOAD,        // Reload stream configuration
    RECONFIGURE,   // Reconfigure plugin
    ABOUT_TO_KILL, // Plugin did not closed when stop called. This is the last warning.
    CUSTOM_REQUEST // Custom request reserved for plugins specific commands
};

class ServiceRequest {
    uint64_t  seekLocation;
    std::string customRequest;
public:
    ServiceRequestEnum type;
    std::string uri;
};

#endif //STREAMLINK_SERVICEREQUEST_H
