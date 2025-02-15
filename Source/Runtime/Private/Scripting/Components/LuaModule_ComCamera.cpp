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
#include "Render/Render.h"
#include "Components/ComCamera.h"

BE_NAMESPACE_BEGIN

void LuaVM::RegisterCameraComponent(LuaCpp::Module &module) {
    LuaCpp::Selector _ComCamera = module["ComCamera"];

    _ComCamera.SetClass<ComCamera>(module["Component"]);
    _ComCamera.AddClassMembers<ComCamera>(
        "size", &ComCamera::GetSize,
        "ortho_size", &ComCamera::GetOrthoSize,
        "aspect_ratio", &ComCamera::GetAspectRatio,
        "world_to_screen_point", &ComCamera::WorldToScreenPoint,
        "screen_point_to_ray", &ComCamera::ScreenPointToRay);

    _ComCamera["meta_object"] = ComCamera::metaObject;
}

BE_NAMESPACE_END
