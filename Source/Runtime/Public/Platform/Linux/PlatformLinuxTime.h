// Copyright(c) 2017 POLYGONTEK
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#define USE_BASE_PLATFORM_POSIX_TIME
#include "../Posix/PlatformPosixTime.h"

BE_NAMESPACE_BEGIN

class BE_API PlatformLinuxTime : public PlatformPosixTime {
public:
    static void             Init();

    static double           Seconds();

    static uint64_t         Cycles();
};

typedef PlatformLinuxTime   PlatformTime;

BE_NAMESPACE_END
