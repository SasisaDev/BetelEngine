#pragma once

#include <vector>

#include <World/Entity.h>
#include "TilemapLayer.h"

class TilemapChunkRenderProxy : public EntityRenderProxy
{
    Entity* Parent;
public:
    TilemapChunkRenderProxy(Entity* DefaultParent) : EntityRenderProxy(DefaultParent) {}
    virtual void Update() override{}
    virtual void Render(VkCommandBuffer cmdBuffer) override;
};

class TilemapChunkEntity : public Entity
{
    std::vector<TilemapLayer*> layers;
public:
    virtual EntityRenderProxy* CreateRenderProxy() override {return RenderProxy = new TilemapChunkRenderProxy(this);}
};