#pragma once
#include <RenderV/Vulkan/vkloader.h>

#include <set>
#include <concepts>
#include <type_traits>

#include <Math/Transform.h>

class EntityRenderProxy;
class World;

struct EntitySpawnInfo
{

};

class Entity
{
protected:
    Transform transform;

    World* OwnerWorld;
    Entity* Parent;
    std::set<Entity*> Children;
    EntityRenderProxy* RenderProxy;
public:
    virtual EntityRenderProxy* CreateRenderProxy(){return RenderProxy = nullptr;}
    virtual EntityRenderProxy* GetRenderProxy() {return RenderProxy;}

    virtual inline World* GetWorld() const {return OwnerWorld;}

    virtual void SetTransform(const Transform& newTransform){transform = newTransform;}
    virtual const Transform& GetTransform() const {return transform;}
};

class WorldRenderLayerRef;

class EntityRenderProxy
{
    Entity* Parent;
public:
    // Specifies whether entity gets automatically loaded/unloaded from memory
    bool bPersistent = false;
public:
    EntityRenderProxy(Entity* DefaultParent){Parent = DefaultParent;}
    virtual void Update() {}
    virtual void Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef) {}
};

template<typename T>
concept EntityClass = std::is_base_of_v<Entity, T>;