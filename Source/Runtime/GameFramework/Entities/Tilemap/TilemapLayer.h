#pragma once

#include <vector>

#include <World/Entity.h>

#include <RenderV/Objects/Buffers/Buffer.h>

class EntTilemapChunk;

struct TilemapLayerData {
    Buffer* UBO;

    virtual std::string GetType() {return "TilemapLayerData";}
};

class TilemapLayer
{
    
public:
    virtual TilemapLayerData* CreateLayerData() {return new TilemapLayerData;}

    virtual void CreateResources(WorldRenderLayerRef* layerRef, EntTilemapChunk* chunk, TilemapLayerData* data){}
    virtual void PrepareFrame(WorldRenderLayerRef* layerRef, EntTilemapChunk* chunk, TilemapLayerData* data){}
    virtual void Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef, EntTilemapChunk* chunk, TilemapLayerData* data){}
};