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

// Buffer pool logs
#define LOG_BUFFER_POOL            (1 << 0)
// Buffer throttle logs
#define LOG_BUFFER_THROTTLE        (1 << 1)
// Configuration read/writes
#define LOG_CONFIGS                (1 << 2)
// Configuration data source logs
#define LOG_DATA_SRC               (1 << 3)
// Configuration data parser logs (PSI parser)
#define LOG_DATA_PARSER            (1 << 4)
// Plugin management logs
#define LOG_PLUGIN_MGR             (1 << 5)
// Video protection logs
#define LOG_PROTECTION             (1 << 6)
