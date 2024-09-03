#pragma once
#include "Entity.h"

#include <vector>
#include <string>

class World
{
protected:
    std::vector<Entity*> entities;
public:
    World();

    template<EntityClass EntityType>
    EntityType* Spawn(std::string name, const EntitySpawnInfo& spawnInfo) 
    {
        EntityType* spawnedEntity = new EntityType();
        spawnedEntity->CreateRenderProxy();
        entities.push_back(spawnedEntity);
        return spawnedEntity;
    }

    inline std::vector<Entity*> GetEntities() const {return entities;}

    virtual void Tick(float DeltaTime);
};