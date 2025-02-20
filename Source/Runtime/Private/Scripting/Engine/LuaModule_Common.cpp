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

#include "Precompiled.h"
#include "Scripting/LuaVM.h"
#include "Engine/Common.h"

BE_NAMESPACE_BEGIN

struct CommonPlatformId {};

void LuaVM::RegisterCommon(LuaCpp::Module &module) {
    LuaCpp::Selector _Common = module["Common"];

    _Common.SetObj(common);
    _Common.AddObjMembers(common,
        "real_time", &Common::realTime,
        "frame_time", &Common::frameTime,
        "with_editor", &Common::WithEditor,
        "platform_id", &Common::GetPlatformId,
        "preference_dir", &Common::GetAppPreferenceDir);

    LuaCpp::Selector _Common_PlatformId = _Common["PlatformId"];
    _Common_PlatformId.SetClass<CommonPlatformId>();
    _Common_PlatformId["Windows"] = Common::PlatformId::WindowsId;
    _Common_PlatformId["Linux"] = Common::PlatformId::LinuxId;
    _Common_PlatformId["MacOS"] = Common::PlatformId::MacOSId;
    _Common_PlatformId["IOS"] = Common::PlatformId::IOSId;
    _Common_PlatformId["Android"] = Common::PlatformId::AndroidId;
}

BE_NAMESPACE_END
