#pragma once

#include <Object/Object.h>

#include <RenderV/Vulkan/vkloader.h>

#include <set>
#include <concepts>
#include <type_traits>
#include <string>

#include <Math/Transform.h>

class EntityRenderProxy;
class World;

struct EntitySpawnInfo
{

};

class Entity : public Object
{
    friend class World;
protected:
    Transform transform;

    EntityRenderProxy* RenderProxy;

    std::string DisplayName = "Entity";

    bool bBeginDestroy = false;
public:
    bool Visible = true;

    void BeginDestroy() {bBeginDestroy = true;}

    virtual EntityRenderProxy* CreateRenderProxy(){return RenderProxy = nullptr;}
    virtual EntityRenderProxy* GetRenderProxy() {return RenderProxy;}

    virtual World* GetWorld();
    
    virtual void Tick(float deltaTime);

    virtual Transform GetTransform();
    virtual const Transform& GetRelativeTransform() const {return transform;}

    virtual IVec3 GetLocation();
    virtual const IVec3& GetRelativeLocation() const {return transform.Location;}

    // TODO
    virtual void SetLocation(const IVec3& loc){}
    virtual void SetRelativeLocation(const IVec3& loc) {transform.Location = loc;}

    virtual std::string GetDisplayName() {return DisplayName;} 
};

class WorldRenderLayerRef;

class EntityRenderProxy
{
public:
    Entity* Parent;

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