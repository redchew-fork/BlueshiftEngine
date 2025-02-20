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
#include "Game/Entity.h"
#include "Asset/Asset.h"
#include "Components/ComTransform.h"
#include "Components/ComCollider.h"
#include "Components/ComBoxCollider.h"
#include "Components/ComSphereCollider.h"
#include "Components/ComCapsuleCollider.h"
#include "Components/ComConeCollider.h"
#include "Components/ComCylinderCollider.h"
#include "Components/ComMeshCollider.h"
#include "Components/ComRigidBody.h"
#include "Components/ComSensor.h"
#include "Components/ComVehicleWheel.h"
#include "Components/ComConstantForce.h"
#include "Components/ComJoint.h"
#include "Components/ComFixedJoint.h"
#include "Components/ComHingeJoint.h"
#include "Components/ComSocketJoint.h"
#include "Components/ComSliderJoint.h"
#include "Components/ComSpringJoint.h"
#include "Components/ComWheelJoint.h"
#include "Components/ComCharacterJoint.h"
#include "Components/ComCharacterController.h"
#include "Components/ComCamera.h"
#include "Components/ComCanvas.h"
#include "Components/ComLight.h"
#include "Components/ComRenderable.h"
#include "Components/ComMeshRenderer.h"
#include "Components/ComStaticMeshRenderer.h"
#include "Components/ComSkinnedMeshRenderer.h"
#include "Components/ComAnimation.h"
#include "Components/ComAnimator.h"
#include "Components/ComTextRenderer.h"
#include "Components/ComText.h"
#include "Components/ComImage.h"
#include "Components/ComParticleSystem.h"
#include "Components/ComLogic.h"
#include "Components/ComSpline.h"
#include "Components/ComScript.h"
#include "Components/ComAudioListener.h"
#include "Components/ComAudioSource.h"

BE_NAMESPACE_BEGIN

void LuaVM::RegisterObject(LuaCpp::Module &module) {
    LuaCpp::Selector _MetaObject = module["MetaObject"];

    _MetaObject.SetClass<MetaObject>();
    _MetaObject.AddClassMembers<MetaObject>(
        "classname", &MetaObject::ClassName,
        "super_classname", &MetaObject::SuperClassName,
        "create_instance", static_cast<Object*(MetaObject::*)()const>(&MetaObject::CreateInstance),
        "is_type_of", &MetaObject::IsTypeOf);

    LuaCpp::Selector _SignalObject = module["SignalObject"];
    
    _SignalObject.SetClass<SignalObject>();

    LuaCpp::Selector _Object = module["Object"];
    _Object.SetClass<Object>();
    _Object.AddClassMembers<Object>(
        "instance_id", &Object::GetInstanceID,
        "classname", &Object::ClassName,
        "super_classname", &Object::SuperClassName,
        "cast_entity", static_cast<Entity*(Object::*)()>(&Object::Cast<Entity>),
        "cast_component", static_cast<Component*(Object::*)()>(&Object::Cast<Component>),
        "cast_transform", static_cast<ComTransform*(Object::*)()>(&Object::Cast<ComTransform>),
        "cast_collider", static_cast<ComCollider*(Object::*)()>(&Object::Cast<ComCollider>),
        "cast_box_collider", static_cast<ComBoxCollider*(Object::*)()>(&Object::Cast<ComBoxCollider>),
        "cast_sphere_collider", static_cast<ComSphereCollider*(Object::*)()>(&Object::Cast<ComSphereCollider>),
        "cast_capsule_collider", static_cast<ComCapsuleCollider*(Object::*)()>(&Object::Cast<ComCapsuleCollider>),
        "cast_cone_collider", static_cast<ComConeCollider*(Object::*)()>(&Object::Cast<ComConeCollider>),
        "cast_cylinder_collider", static_cast<ComCylinderCollider*(Object::*)()>(&Object::Cast<ComCylinderCollider>),
        "cast_mesh_collider", static_cast<ComMeshCollider*(Object::*)()>(&Object::Cast<ComMeshCollider>),
        "cast_rigid_body", static_cast<ComRigidBody*(Object::*)()>(&Object::Cast<ComRigidBody>),
        "cast_sensor", static_cast<ComSensor*(Object::*)()>(&Object::Cast<ComSensor>),
        "cast_vehicle_wheel", static_cast<ComVehicleWheel*(Object::*)()>(&Object::Cast<ComVehicleWheel>),
        "cast_constant_force", static_cast<ComConstantForce*(Object::*)()>(&Object::Cast<ComConstantForce>),
        "cast_joint", static_cast<ComJoint*(Object::*)()>(&Object::Cast<ComJoint>),
        "cast_fixed_joint", static_cast<ComFixedJoint*(Object::*)()>(&Object::Cast<ComFixedJoint>),
        "cast_hinge_joint", static_cast<ComHingeJoint*(Object::*)()>(&Object::Cast<ComHingeJoint>),
        "cast_socket_joint", static_cast<ComSocketJoint*(Object::*)()>(&Object::Cast<ComSocketJoint>),
        "cast_slider_joint", static_cast<ComSliderJoint*(Object::*)()>(&Object::Cast<ComSliderJoint>),
        "cast_spring_joint", static_cast<ComSpringJoint*(Object::*)()>(&Object::Cast<ComSpringJoint>),
        "cast_wheel_joint", static_cast<ComWheelJoint*(Object::*)()>(&Object::Cast<ComWheelJoint>),
        "cast_character_joint", static_cast<ComCharacterJoint*(Object::*)()>(&Object::Cast<ComCharacterJoint>),
        "cast_character_controller", static_cast<ComCharacterController*(Object::*)()>(&Object::Cast<ComCharacterController>),
        "cast_camera", static_cast<ComCamera*(Object::*)()>(&Object::Cast<ComCamera>),
        "cast_canvas", static_cast<ComCanvas*(Object::*)()>(&Object::Cast<ComCanvas>),
        "cast_light", static_cast<ComLight*(Object::*)()>(&Object::Cast<ComLight>),
        "cast_renderable", static_cast<ComRenderable*(Object::*)()>(&Object::Cast<ComRenderable>),
        "cast_mesh_renderer", static_cast<ComMeshRenderer*(Object::*)()>(&Object::Cast<ComMeshRenderer>),
        "cast_static_mesh_renderer", static_cast<ComStaticMeshRenderer*(Object::*)()>(&Object::Cast<ComStaticMeshRenderer>),
        "cast_skinned_mesh_renderer", static_cast<ComSkinnedMeshRenderer*(Object::*)()>(&Object::Cast<ComSkinnedMeshRenderer>),
        "cast_animation", static_cast<ComAnimation*(Object::*)()>(&Object::Cast<ComAnimation>),
        "cast_animator", static_cast<ComAnimator*(Object::*)()>(&Object::Cast<ComAnimator>),
        "cast_text_renderer", static_cast<ComTextRenderer*(Object::*)()>(&Object::Cast<ComTextRenderer>),
        "cast_text", static_cast<ComText*(Object::*)()>(&Object::Cast<ComText>),
        "cast_image", static_cast<ComImage*(Object::*)()>(&Object::Cast<ComImage>),
        "cast_particle_system", static_cast<ComParticleSystem*(Object::*)()>(&Object::Cast<ComParticleSystem>),
        "cast_audio_source", static_cast<ComAudioSource*(Object::*)()>(&Object::Cast<ComAudioSource>),
        "cast_audio_listener", static_cast<ComAudioListener*(Object::*)()>(&Object::Cast<ComAudioListener>),
        "cast_spline", static_cast<ComSpline*(Object::*)()>(&Object::Cast<ComSpline>),
        "cast_script", static_cast<ComScript*(Object::*)()>(&Object::Cast<ComScript>),
        "cast_asset", static_cast<Asset*(Object::*)()>(&Object::Cast<Asset>));

    _Object["meta_object"] = Object::metaObject;
    _Object["destroy"].SetFunc(&Object::DestroyInstance);
}

BE_NAMESPACE_END
