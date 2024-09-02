#pragma once
#include <RenderV/Vulkan/vkloader.h>

#include <set>
#include <concepts>
#include <type_traits>

class EntityRenderProxy;
class World;

struct EntitySpawnInfo
{

};

class Entity
{
protected:
    World* OwnerWorld;
    Entity* Parent;
    std::set<Entity*> Children;
    EntityRenderProxy* RenderProxy;
public:
    virtual EntityRenderProxy* CreateRenderProxy(){return RenderProxy = nullptr;}
    virtual EntityRenderProxy* GetRenderProxy() {return RenderProxy;}

    virtual inline World* GetWorld() const {return OwnerWorld;}
};

class EntityRenderProxy
{
    Entity* Parent;
public:
    virtual void Update() {}
    virtual void Render(VkCommandBuffer cmdBuffer) {}
};

template<typename T>
concept EntityClass = std::is_base_of_v<Entity, T>;