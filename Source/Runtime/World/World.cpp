#include "World.h"

World::World()
{
    
}

void World::Tick(float DeltaTime)
{
    for(Entity* entity : entities) {
        entity->Tick(DeltaTime);
    }
}