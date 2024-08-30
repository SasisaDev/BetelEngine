#pragma once
#include "Entity.h"

#include <vector>

class World
{
protected:
    std::vector<Entity*> entities;
public:
    World();

    inline std::vector<Entity*> GetEntities() const {return entities;}
};