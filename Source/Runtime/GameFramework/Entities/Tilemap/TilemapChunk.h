#pragma once

#include <vector>

#include <World/Entity.h>
#include "TilemapLayer.h"

class TilemapChunkRenderProxy : public EntityRenderProxy
{
public:
    TilemapChunkRenderProxy(Entity* DefaultParent) : EntityRenderProxy(DefaultParent) {}
    virtual void Update(WorldRenderLayerRef* layerRef) {}
    virtual void Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef) override;
};

class TilemapChunkEntity : public Entity
{
    std::vector<TilemapLayer*> layers;
public:
    virtual EntityRenderProxy* CreateRenderProxy() override {return RenderProxy = new TilemapChunkRenderProxy(this);}
};