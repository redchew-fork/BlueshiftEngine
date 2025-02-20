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

/*
-------------------------------------------------------------------------------

    Entity

-------------------------------------------------------------------------------
*/

#include "Containers/HashTable.h"
#include "Core/Object.h"
#include "Render/RenderCamera.h"
#include "Components/Component.h"

BE_NAMESPACE_BEGIN

class Component;
class ComTransform;
class ComRectTransform;
class GameWorld;
class Prefab;
class Entity;

using EntityPtr = Entity*;
using EntityPtrArray = Array<EntityPtr>;

/// An Entity is any object that can be placed into a scene.
class Entity : public Object {
    friend class GameWorld;
    friend class Prefab;
    friend class Component;

public:
    struct WorldPosTrait {
        enum Enum {
            Pivot,
            Center,
            Minimum,
            Maximum
        };
    };

    OBJECT_PROTOTYPE(Entity);
    
    Entity();
    virtual ~Entity();

    virtual Str                 ToString() const override { return GetName(); }
    
                                /// Returns name.
    Str                         GetName() const { return name; }
                                /// Sets name.
    void                        SetName(const Str &name);

                                /// Returns tag name.
    Str                         GetTag() const { return tag; }
                                /// Sets tag name.
    void                        SetTag(const Str &tag);

                                /// Returns layer index.
    int                         GetLayer() const { return layer; }
                                /// Sets layer index.
    void                        SetLayer(int layer);

                                /// Returns static mask.
    int                         GetStaticMask() const { return staticMask; }
                                /// Sets static mask.
    void                        SetStaticMask(int staticMask);

#if WITH_EDITOR
                                /// Returns true if this entity is visible in editor.
    bool                        IsVisible() const { return visibleInEditor; }
                                /// Sets this entity's visiblilty in editor.
    void                        SetVisible(bool visible);
                                /// Returns true if this entity is selectable in editor.
    bool                        IsSelectable() const { return selectableInEditor; }
                                /// Sets this entity's selectablilty in editor.
    void                        SetSelectable(bool selectable);
#endif

                                /// Returns true if this entity is a prefab source.
    bool                        IsPrefabSource() const { return prefab; }

    Guid                        GetPrefabSourceGuid() const;
    void                        SetPrefabSourceGuid(const Guid &prefabSourceGuid);

                                /// Returns GameWorld of this entity is in.
    GameWorld *                 GetGameWorld() const { return gameWorld; }

    int                         GetEntityNum() const { return entityNum; }

    int                         GetSceneNum() const { return sceneNum; }

                                /// Returns hierarchy node.
    const Hierarchy<Entity> &   GetNode() const { return node; }

                                /// Returns root entity.
    Entity *                    GetRoot() const;
                                /// Returns true if this entity is a root entity.
    bool                        IsRoot() const { return GetRoot() == this; }

                                /// Returns parent entity.
    Entity *                    GetParent() const { return node.GetParent(); }
                                /// Sets parent entity.
    void                        SetParent(Entity *parentEntity);

                                /// Returns parent entity GUID.
    Guid                        GetParentGuid() const;
                                /// Sets parent entity by GUID.
    void                        SetParentGuid(const Guid &parentGuid);

                                /// Returns sibling index.
    int                         GetSiblingIndex() const;
                                /// Sets sibling index.
    void                        SetSiblingIndex(int index);

                                /// Returns true if this entity have any children.
    bool                        HasChildren() const;
                                /// Returns the number of children the parent entity has.
    int                         GetChildCount() const;
                                /// Returns a child entity by index.
    Entity *                    GetChild(int childIndex) const;
                                /// Finds a child entity with the given name.
    Entity *                    FindChild(const char *name) const;
                                /// Gets the children entities.
    void                        GetChildren(EntityPtrArray &children) const;
                                /// Gets all of the children by depth first order.
    void                        GetChildrenRecursive(EntityPtrArray &children) const;

                                /// Returns number of components.
    int                         NumComponents() const { return components.Count(); }
                                /// Checks if component exist by the given meta object.
    bool                        HasComponent(const MetaObject *type) const;
                                /// Returns a component pointer that is conflicting with other components.
    Component *                 GetConflictingComponent(const MetaObject *type) const;
                                /// Returns index of the component pointer.
    int                         GetComponentIndex(const Component *component) const;
                                /// Returns a component pointer by the given comopnent index.
    Component *                 GetComponent(int index) const { return components[index]; }
                                /// Returns a component pointer by the given meta object.
    Component *                 GetComponent(const MetaObject *type) const;
                                /// Returns a component pointer by the given type T.
    template <typename T> T *   GetComponent() const;
                                /// Returns a component pointer by the given type T.
    template <typename T> T *   GetComponent(int index) const;
                                /// Returns all component pointers.
    ComponentPtrArray &         GetComponents() { return components; }
                                /// Returns all component pointers by the given meta object.
    ComponentPtrArray           GetComponents(const MetaObject *type) const;
                                /// Returns all component pointers by the given meta object in this entity or any children.
    ComponentPtrArray           GetComponentsInChildren(const MetaObject *type, bool skipIfParentDontHave = false) const;
                                /// Returns all component pointers by the given type T in this entity or any children.
    template <typename T> 
    ComponentPtrArray           GetComponentsInChildren(bool skipIfParentDontHave = false) const;

                                /// Returns a transform component.
    ComTransform *              GetTransform() const;
                                /// Returns a rect transform component.
    ComRectTransform *          GetRectTransform() const;

                                /// Returns an entity has rect transform which is hit by ray. 
    Entity *                    RayCastRect(const Ray &ray);

                                /// Adds a component to the entity.
    void                        AddComponent(Component *component) { InsertComponent(component, components.Count()); }
                                /// Inserts a component after the index to the entity.
    void                        InsertComponent(Component *component, int index);
                                /// Removes a component.
    bool                        RemoveComponent(Component *component);
                                /// Swap two components.
    bool                        SwapComponent(int fromIndex, int toIndex);

                                /// Adds new component to the entity in real-time.
    Component *                 AddNewComponent(const MetaObject *type);

                                /// Returns if any components of this entity have render object by checking given render object handle.
    bool                        HasRenderObject(int renderObjectHandle) const;

                                /// Purges all of the data.
    void                        Purge();

                                /// Initializes this entity. Always parent entities will be initialized first.
    void                        Init();
                                /// Initializes components.
    void                        InitComponents();

                                /// Called once when game started before Start()
                                /// When game already started, called immediately after spawned
    void                        Awake();
                                /// Called once when game started.
                                /// When game already started, called immediately after spawned
    void                        Start();

                                /// Called on game world update, variable timestep.
    void                        Update();
                                /// Called on game world late-update, variable timestep.
    void                        LateUpdate();
                                /// Called on physics update, fixed timestep.
    void                        FixedUpdate(float timeStep);
                                /// Called on physics late-update, fixed timestep.
    void                        FixedLateUpdate(float timeStep);

                                /// Serializes entity to JSON value.
    virtual void                Serialize(Json::Value &data, bool forCopying = false) const override;
                                /// Deserializes entity from JSON value.
    virtual void                Deserialize(const Json::Value &data) override;
                                /// Serializes given entity hierarchy to JSON value.
    static void                 SerializeHierarchy(const Entity *entity, Json::Value &entitiesValue, bool forCopying = false);

                                /// Returns if this entity is active. 
                                /// Note that an entity may be inactive because a parent is not active, even if this returns true.
                                /// Use IsActiveInHierarchy() if you want to check if the entity is actually treated as active in the scene.
    bool                        IsActiveSelf() const { return activeSelf; }
                                /// Returns if this entity is active in the game.
    bool                        IsActiveInHierarchy() const { return activeInHierarchy; }

                                /// Sets local active state of this entity.
    void                        SetActive(bool active);

                                /// Returns AABB in local space.
    const AABB                  GetLocalAABB(bool includingChildren = false) const;
                                /// Returns AABB in world space exactly. This is slow than GetWorldAABB().
    const AABB                  GetWorldAABB(bool includingChildren = false) const;
                                /// Returns AABB in world space.
    const AABB                  GetWorldAABBFast(bool includingChildren = false) const;
                                /// Returns AABB in the given space.
    const AABB                  GetAABBInSpace(const Vec3 &origin, const Mat3 &axis, bool includingChildren = false) const;
                                /// Returns position in world space with given trait.
    const Vec3                  GetWorldPosition(WorldPosTrait::Enum posTrait, bool includingChildren = false) const;

#if WITH_EDITOR
                                /// Visualizes the component in editor.
    void                        DrawGizmos(const RenderCamera *camera, bool selected, bool selectedByParent);
#endif

                                /// Ray cast to this entity.
    bool                        IntersectRay(const Ray &ray, bool backFaceCull, float &lastDist) const;

                                /// Creates an entity by JSON text.
    static Entity *             CreateEntity(Json::Value &data, GameWorld *gameWorld = nullptr, int sceneIndex = 0);

                                /// Makes copy of JSON value of an entity and then replace each GUIDs of entity/components to the new ones.
    static Json::Value          CloneEntityValue(const Json::Value &entityValue, HashTable<Guid, Guid> &oldToNewGuidMap);
                                /// Makes copy of JSON value of entities and then replace each GUIDs of entity/components to the new ones.
    static Json::Value          CloneEntitiesValue(const Json::Value &entitiesValue, HashTable<Guid, Guid> &oldToNewGuidMap);

                                /// Replaces GUIDs of an entity (including components) using GUID map.
    void                        RemapGuids(const HashTable<Guid, Guid> &guidMap);
        
                                /// Destroys given entity and it's children.
    static void                 DestroyInstance(Entity *entity);

    static const SignalDef      SIG_LayerChanged;
    static const SignalDef      SIG_StaticMaskChanged;

#if WITH_EDITOR
    static const SignalDef      SIG_ActiveChanged;
    static const SignalDef      SIG_ActiveInHierarchyChanged;
    static const SignalDef      SIG_NameChanged;
    static const SignalDef      SIG_VisibilityChanged;
    static const SignalDef      SIG_SelectabilityChanged;
    static const SignalDef      SIG_ParentChanged;
    static const SignalDef      SIG_SiblingIndexChanged;
    static const SignalDef      SIG_ComponentInserted;
    static const SignalDef      SIG_ComponentRemoved;
    static const SignalDef      SIG_ComponentSwapped;
#endif

protected:
    void                        SetActiveInHierarchy(bool active);

    virtual void                Event_ImmediateDestroy() override;

                                /// Called when the application resizes.
    void                        OnApplicationResize(int width, int height);
                                /// Called when the application terminates.
    void                        OnApplicationTerminate();
                                /// Called when the application pauses.
    void                        OnApplicationPause(bool pause);

    Str                         name;               ///< Entity name
    int                         nameHash;           ///< Hash key for GameWorld::entityHash
    Str                         tag;                ///< Tag name
    int                         tagHash;            ///< Hash key for GameWorld::entityTagHash
    int                         entityNum;          ///< Index for GameWorld::entities
    int                         layer;              ///< Layer number
    int                         staticMask;
    Hierarchy<Entity>           node;
    Guid                        prefabSourceGuid;

    bool                        initialized;
    bool                        awaked;
    bool                        started;
    bool                        activeSelf;         ///< Local active state
    bool                        activeInHierarchy;  ///< Actual active state 
    bool                        prefab;

#if WITH_EDITOR
    bool                        visibleInEditor = true;
    bool                        selectableInEditor = true;
#endif

    GameWorld *                 gameWorld;
    int                         sceneNum = -1;

    ComponentPtrArray           components;         ///< 0'th component is always transform component
};

template <typename T>
BE_INLINE T *Entity::GetComponent() const {
    Component *component = GetComponent(&T::metaObject);
    if (component) {
        return component->Cast<T>();
    }

    return nullptr;
}

template <typename T>
BE_INLINE T *Entity::GetComponent(int index) const {
    ComponentPtrArray components = GetComponents(&T::metaObject);
    int numComponents = components.Count();
    if (numComponents > 0 && index < numComponents) {
        return components[index]->Cast<T>();
    }

    return nullptr;
}

BE_INLINE bool Entity::HasComponent(const MetaObject *type) const {
    if (GetComponent(type)) {
        return true;
    }
    return false;
}

BE_INLINE int Entity::GetComponentIndex(const Component *component) const {
    return components.FindIndex(const_cast<Component *>(component));
}

template <typename T>
BE_INLINE ComponentPtrArray Entity::GetComponentsInChildren(bool skipIfParentDontHave) const {
    ComponentPtrArray subComponents = GetComponentsInChildren(&T::metaObject, skipIfParentDontHave);

    return subComponents;
}

BE_INLINE Component *Entity::GetConflictingComponent(const MetaObject *type) const {
    for (int i = 0; i < components.Count(); i++) {
        Component *component = components[i];
        if (component->IsConflictComponent(*type)) {
            return component;
        }
    }

    return nullptr;
}

BE_INLINE Entity *Entity::GetRoot() const {
    const Entity *ent = this;
    while (const Entity *parent = ent->GetParent()) {
        ent = parent;
    }
    return const_cast<Entity *>(ent);
}

BE_NAMESPACE_END
