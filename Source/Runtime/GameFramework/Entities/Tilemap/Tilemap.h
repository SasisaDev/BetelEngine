#pragma once

#include <vector>
#include <unordered_map>

#include <World/Entity.h>
#include "TilemapChunk.h"
#include <Object/ObjectRef.h>
#include <Math/Vector.h>

#include <Utility/Concepts.h>

struct TilemapGPUData {
    uint32_t ChunkWidth = 0;
    uint32_t TileSize = 0;
};

struct TilemapChunkData 
{
    ObjectRef<EntTilemapChunk> Chunk;
    bool AlwaysLoaded = false;
};

class EntTilemap : public Entity
{
    std::map<IVec2, TilemapChunkData> chunks;
    std::vector<TilemapLayer*> layers;
public:

    template <Derived<TilemapLayer> LayerType>
    LayerType* AddLayer() {
        LayerType* layer = new LayerType;
        layers.push_back(layer);
        return layer;
    }

    static std::string GetStaticType() {return "TILEMAP";}

    std::vector<TilemapLayer*>& GetLayers() {return layers;}

    void RemoveLayer(size_t index); 

    ObjectRef<EntTilemapChunk>& CreateChunk(IVec2 position);
};

class EntTilemapType : public EntityType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new EntTilemap; }
    virtual bool ShowInEditor() override { return false; }
    virtual std::string_view DisplayName() override {return "Tilemap";}
};
