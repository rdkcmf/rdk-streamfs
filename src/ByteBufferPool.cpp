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

#include "streamfs/ByteBufferPool.h"

#include <utility>

void ByteBufferPool::pushBuffer(const buffer_chunk &buffer, bool lastBuffer, size_t lastBufferSize) {
    BufferPool::pushBuffer(buffer, lastBuffer, lastBufferSize);
}

ByteBufferPool::ByteBufferPool(ByteBufferPool::shared_producer_type pProducer,
                               ByteBufferPool::shared_consumer_type pConsumer, uint64_t i) :
        BufferPool(std::move(pProducer), std::move(pConsumer), i) {
}

size_t ByteBufferPool::read(char *bufferChunks, size_t length, uint64_t offset,
                            size_t left_padding,
                            size_t right_padding
) {
    return BufferPool::read(bufferChunks, length, offset, left_padding, right_padding);
}


size_t ByteBufferPool::readRandomAccess(char *data, size_t size, uint64_t offsetBytes) {
    return BufferPool::readRandomAccess(data, size, offsetBytes);
}

void ByteBufferPool::clear() {
    BufferPool::clear();
}

void ByteBufferPool::abortAllOperations() {
    BufferPool::abortAllOperations();
}
