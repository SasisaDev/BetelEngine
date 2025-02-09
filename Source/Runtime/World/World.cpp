#include "World.h"

World::World()
{
    
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
            entities.erase(entities.begin() + entID);
            --entID;
            continue;
        }

        entities[entID].Get()->Tick(DeltaTime);
    }
}