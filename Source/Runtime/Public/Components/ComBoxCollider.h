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

#include "ComCollider.h"

BE_NAMESPACE_BEGIN

class LuaVM;

class ComBoxCollider : public ComCollider {
    friend class LuaVM;

public:
    OBJECT_PROTOTYPE(ComBoxCollider);

    ComBoxCollider();
    virtual ~ComBoxCollider();

                            /// Initializes this component. Called after deserialization.
    virtual void            Init() override;

#if WITH_EDITOR
                            /// Visualize the component in editor
    virtual void            DrawGizmos(const RenderCamera *camera, bool selected, bool selectedByParent) override;
#endif

    Vec3                    GetCenter() const { return center; }
    void                    SetCenter(const Vec3 &center);

    Vec3                    GetExtents() const { return extents; }
    void                    SetExtents(const Vec3 &extents);

protected:
    virtual void            CreateCollider() override;

    Vec3                    center;     ///< Local center position in system units
    Vec3                    extents;    ///< Local size for each axis in system units
};

BE_NAMESPACE_END
