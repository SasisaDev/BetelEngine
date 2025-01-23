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

    for(int entID = 0; entID < entities.size(); entID++) {
        // Remove destruction pending entity
        if(entities[entID].Get()->bBeginDestroy) {
            entities.erase(entities.begin() + entID);
            entID++;
        }

        entities[entID].Get()->Tick(DeltaTime);
    }
}