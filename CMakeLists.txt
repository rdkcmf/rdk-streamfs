#
# If not stated otherwise in this file or this component's LICENSE
# file the following copyright and licenses apply:
#
# Copyright (c) 2022 Nuuday.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.10)
project(streamlink)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set( CMAKE_VERBOSE_MAKEFILE on )

option(BUFFER_CHUNK_READ_THROTTLING "Enable buffer chunk read throttling from the BufferPool" ON)
if (BUFFER_CHUNK_READ_THROTTLING)
        add_definitions(-DBUFFER_CHUNK_READ_THROTTLING)
endif()

include(CheckFileOffsetBits)

CHECK_FILE_OFFSET_BITS()
# fuse defines
ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64)

set(PUBLIC_HEADERS
        export/streamfs/BufferConsumer.h
        export/streamfs/BufferPool.h
        export/streamfs/BufferProducer.h
        export/streamfs/ByteBufferPool.h
        export/streamfs/config.h
        export/streamfs/FileInterface.h
        export/streamfs/MessageQueue.h
        export/streamfs/PluginCallbackInterface.h
        export/streamfs/PluginConfig.h
        export/streamfs/PluginInterface.h
        export/streamfs/PluginService.h
        export/streamfs/ServiceRequest.h
        export/streamfs/LogLevels.h
        export/streamfs/DebugOptions.h
)

include_directories(
        include
        export
        logging/src
        )

add_library(buqu SHARED
        src/BufferPool.cpp export/streamfs/BufferPool.h
        src/BufferPoolThrottle.cpp
        src/BufferProducer.cpp
        src/BufferPool.cpp
        export/streamfs/BufferProducer.h
        src/BufferConsumer.cpp
        src/ByteBufferPool.cpp
        src/Logging.cpp
)

add_library(streamlink SHARED
        src/StreamPluginManager.cpp
        src/PluginManagerConfig.cpp
        src/BufferQueue.cpp
        src/PluginImpl.cpp
        src/MemAllocator.cpp include/streamfs/MemAllocator.h
        src/ActiveBuffer.cpp include/streamfs/ActiveBuffer.h
        src/BufferPool.cpp export/streamfs/BufferPool.h
        src/BufferPoolThrottle.cpp
        src/BufferProducer.cpp
        src/BufferPool.cpp
        export/streamfs/BufferProducer.h
        src/BufferConsumer.cpp
        src/ByteBufferPool.cpp
        src/VirtualFSProvider.cpp
        include/streamfs/VirtualFSProvider.h
        src/fuse/IFuse.cpp
        src/Logging.cpp
        include/fuse/IFuse.h
        export/streamfs/BufferConsumer.h
        src/PluginCbImpl.cpp
        include/streamfs/PluginCbImpl.h
        )

find_package (glog)

add_subdirectory(tests)
# set(Boost_DEBUG 1)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS filesystem chrono)

find_package(FUSE REQUIRED)
find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIR})
include_directories( ${Boost_INCLUDE_DIRS} )


add_library(sampleplugin SHARED
        sample/SamplePlugin.cpp
        export/streamfs/FileInterface.h)

add_dependencies(streamlink sampleplugin)

target_link_libraries (sampleplugin
        ${Boost_LIBRARIES}
        ${CURL_LIBRARIES}
        pthread
        boost_thread
        glog)

target_link_libraries(buqu
        ${Boost_LIBRARIES}
        ${CURL_LIBRARIES}
        pthread
        boost_thread
        glog
        )

target_link_libraries (streamlink
        PUBLIC
        dl
        ${FUSE_LIBRARIES}
        ${Boost_LIBRARIES}
        pthread
        boost_thread
        glog)


add_executable(streamfs main.cpp
        export/streamfs/MessageQueue.h
        src/PluginService.cpp
        export/streamfs/PluginService.h
        )

target_link_libraries (streamfs
        streamlink
        ${Boost_LIBRARIES}
        ${FUSE_LIBRARIES}
        pthread
        boost_thread
        )


set_target_properties(buqu PROPERTIES PUBLIC_HEADER  "${PUBLIC_HEADERS}")

INSTALL(TARGETS buqu
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/streamfs
)

INSTALL(TARGETS streamlink
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/streamfs
)

install (TARGETS streamfs
         RUNTIME DESTINATION bin)

INSTALL(TARGETS sampleplugin
        LIBRARY DESTINATION lib/streamfs/
        )

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/export
        CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fsanitize=address")
