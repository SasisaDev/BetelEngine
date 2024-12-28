#pragma once

#include <vector>

#include <World/Entity.h>
#include "TilemapChunk.h"

class TilemapEntity : public Entity
{
    std::vector<TilemapChunkEntity*> chunks;
    std::vector<TilemapLayer*> layers;
public:
};

class TilemapEntityType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() { return new TilemapEntity; }
};

