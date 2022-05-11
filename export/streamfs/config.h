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

#include <array>
#include <boost/static_assert.hpp>

// Plugin location directory
#define STREAMFS_PLUGIN_DIRECTORY "plugins/"
#define TS_PACKAGE_SIZE 188

// Buffer chunk sizes
#define BUFFER_CHUNK_SIZE (8 * 6 * TS_PACKAGE_SIZE) /* 1k chunks */

BOOST_STATIC_ASSERT( BUFFER_CHUNK_SIZE % TS_PACKAGE_SIZE == 0);

// Max number of buffer chunks in a list
#define MAX_BUFFER_LIST_COUNT 256

// Buffer read timeout in milliseconds
#define BUFFER_POOL_READ_TIMEOUT_MS 4000

using buffer_chunk  = std::array<unsigned char, BUFFER_CHUNK_SIZE>;
