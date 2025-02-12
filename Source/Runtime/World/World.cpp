#include "World.h"

World::World()
{
    
}

void World::DestroyEntity(Entity *entity)
{
    assert(entity != nullptr);

    for(int entID = 0; entID < entities.size(); ++entID) {
        // Remove destruction pending entity
        uint32_t destroyEntityId = entity->GetID();
        uint32_t thisEntityId = entities[entID].GetID();
        if(destroyEntityId == thisEntityId) {
            OnEntityDestroyed.Broadcast(entity);
            entities.erase(entities.begin() + entID);
            break;
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