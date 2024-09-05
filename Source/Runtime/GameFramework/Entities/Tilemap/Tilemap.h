#pragma once

#include <vector>

#include <World/Entity.h>
#include "TilemapChunk.h"

class TilemapEntity : public Entity
{
    std::vector<TilemapChunkEntity*> chunks;
public:
};