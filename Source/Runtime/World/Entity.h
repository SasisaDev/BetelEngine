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
    
    virtual void Tick(float deltaTime);

    virtual Transform GetTransform();
    virtual const Transform& GetRelativeTransform() const {return transform;}

    virtual Vec3 GetLocation();
    virtual const Vec3& GetRelativeLocation() const {return transform.Location;}
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
    virtual void CreateResources(WorldRenderLayerRef* layerRef){}
    virtual void Update(WorldRenderLayerRef* layerRef) {}
    virtual void Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef) {}
};

template<typename T>
concept EntityClass = std::is_base_of_v<Entity, T>;