#pragma once

#include "TilemapLayer.h"

#include <glm/glm.hpp>
#include <cstdint>

struct TilemapLayerAtlasGPUData {
    glm::ivec2 AtlasDimensions;
    glm::ivec2 ChunkPosition;
    uint32_t LayerDepth;
    glm::vec4* TileUVs;
};

struct TilemapLayerAtlasData : public TilemapLayerData {
    TilemapLayerAtlasGPUData GPUData;
    virtual std::string GetStaticType() {return "TilemapLayerAtlasData";}
};

class TilemapLayerAtlas : public TilemapLayer
{

public:
    virtual void CreateResources(WorldRenderLayerRef* layerRef, EntTilemapChunk* chunk, TilemapLayerData* data) override;
    virtual void PrepareFrame(WorldRenderLayerRef* layerRef, EntTilemapChunk* chunk, TilemapLayerData* data) override;
    virtual void Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef, EntTilemapChunk* chunk, TilemapLayerData* data) override;
};
