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

#ifndef STREAMLINK_BYTEBUFFERPOOL_H
#define STREAMLINK_BYTEBUFFERPOOL_H

#include "BufferPool.h"

class ByteBufferPool : public BufferPool<buffer_chunk >{
public:
    typedef std::shared_ptr<BufferProducer<buffer_chunk>>  shared_producer_type;
    typedef std::shared_ptr<BufferConsumer<buffer_chunk>> shared_consumer_type;

    ByteBufferPool(
            shared_producer_type pProducer,
            shared_consumer_type pConsumer,
            uint64_t i);

    size_t read(
            char* bufferChunks,
            size_t length,
            uint64_t offset,
            size_t left_padding,
            size_t right_padding
            ) override;

    size_t readRandomAccess(char* data, size_t size, uint64_t offsetBytes) override ;
    void pushBuffer(const buffer_chunk &buffer,  bool lastBuffer = false, size_t lastBufferSize = 0) override;
    void clear() override;
    void abortAllOperations() override;
};


#endif //STREAMLINK_BYTEBUFFERPOOL_H
