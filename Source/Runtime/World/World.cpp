#include "World.h"

World::World()
{
    
}

void World::DestroyEntity(Entity *entity)
{
    assert(entity != nullptr);

    for(int entID = 0; entID < entities.size(); ++entID) {
        // Remove destruction pending entity
        if(entity->GetID() == entities[entID].GetID()) {
            OnEntityDestroyed.Broadcast(entities[entID].Get());
            entities.erase(entities.begin() + entID);
            --entID;
            continue;
        }
    }
}

void World::BeginPlay()
{

}

void World::Tick(float DeltaTime)
{
    timerManager.Tick(DeltaTime);

    for(int entID = 0; entID < entities.size(); ++entID) {
        // Remove destruction pending entity
        if(entities[entID].Get()->HasEntityFlag(EntityFlags::BeginDestroy)) {
            OnEntityDestroyed.Broadcast(entities[entID].Get());
            entities.erase(entities.begin() + entID);
            --entID;
            continue;
        }

        entities[entID].Get()->Tick(DeltaTime);
    }
}