#pragma once
#include <RenderV/Vulkan/vkloader.h>

#include <set>

class EntityRenderProxy;

class Entity
{
protected:
    Entity* Parent;
    std::set<Entity*> Children;
    EntityRenderProxy* RenderProxy;
public:
    virtual EntityRenderProxy* CreateRenderProxy(){return RenderProxy = nullptr;}
    virtual EntityRenderProxy* GetRenderProxy() {return RenderProxy;}
};

class EntityRenderProxy
{
    Entity* Parent;
public:
    virtual void Update(){}
    virtual void Render(VkCommandBuffer cmdBuffer) {}
};