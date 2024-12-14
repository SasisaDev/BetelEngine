#include "World.h"

World::World()
{
    
}

void World::StartPlay()
{

}

void World::Tick(float DeltaTime)
{
    for(int entID = 0; entID < entities.size(); entID++) {
        // Remove destruction pending entity
        if(entities[entID]->bBeginDestroy) {
            entities.erase(entities.begin() + entID);
            entID++;
        }

        entities[entID]->Tick(DeltaTime);
    }
}