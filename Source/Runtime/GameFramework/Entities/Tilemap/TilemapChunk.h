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

class EntTilemapChunk : public Entity
{
    friend class EntTilemap;
    std::vector<TilemapLayerData*> layersData;
public:
    virtual EntityRenderProxy* SetupRenderProxy(WorldRenderLayerRef* ref) override {return RenderProxy = new TilemapChunkRenderProxy(this);}
};

class EntTilemapChunkType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new EntTilemapChunk; }
    virtual bool ShowInEditor() override { return false; }
    virtual std::string_view DisplayName() override {return "TilemapChunk";}
};

