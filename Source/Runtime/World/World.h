#pragma once
#include "Entity.h"

#include <vector>
#include <string>
#include <Math/Transform.h>
#include <Delegate/Delegate.h>

class World
{
    friend class WorldRenderLayer;
protected:
    std::vector<Entity*> entities;

    bool BackgroundGradient = false;
    Vec3 BackgroundColor;
    std::pair<Vec3, Vec3> BackgroundGradientColor;

    std::string Name = "Persistent World";
public:
    World();

    MulticastDelegate<Entity*> OnEntitySpawned;
    MulticastDelegate<Entity*> OnEntityDestroyed;

    void SetBackgroundColor(const Vec3& color) {BackgroundColor = color;}

    void SetWorldName(std::string name) {Name = name;}
    std::string GetWorldName() const {return Name;}

    template<EntityClass EntityType>
    EntityType* Spawn(std::string name, const EntitySpawnInfo& spawnInfo)
    {
        EntityType* spawnedEntity = new EntityType();
        spawnedEntity->DisplayName = name;
        entities.push_back(spawnedEntity);

        OnEntitySpawned.Broadcast(spawnedEntity);

        return spawnedEntity;
    }

    inline std::vector<Entity*> GetEntities() const {return entities;}

    virtual void Tick(float DeltaTime);
};