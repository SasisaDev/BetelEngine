#pragma once

#include <vector>
#include <unordered_map>

#include <World/Entity.h>
#include "TilemapChunk.h"
#include <Object/ObjectRef.h>
#include <Math/Vector.h>

struct TilemapChunkData 
{
    bool AlwaysLoaded = false;
    ObjectRef<EntTilemapChunk> Chunk;
};

class EntTilemap : public Entity
{
    std::map<IVec2, TilemapChunkData> chunks;
    std::vector<TilemapLayer*> layers;
public:
};

class EntTilemapType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() { return new EntTilemap; }
};

