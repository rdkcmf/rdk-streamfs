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

#ifndef STREAMFS_BUFFERCONSUMER_H
#define STREAMFS_BUFFERCONSUMER_H

template  <class T>
class BufferConsumer {
public:
    /**
     * Consumer is notified for new buffer when new buffer is available.
     * This buffer is buffer is used when playing back head of the stream without
     * seek.
     * @param buffer - front buffer of circular buffer
     */
    virtual void newBufferAvailable(T& buffer) = 0;
    virtual ~BufferConsumer() {};

};


#endif //STREAMFS_BUFFERCONSUMER_H
