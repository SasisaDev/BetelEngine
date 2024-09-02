#include "Engine.h"

Engine::Engine()
{
    // Create default world
    world = new World;
}

void Engine::Tick(float DeltaTime)
{
    tickManager.Tick(DeltaTime);

    world->Tick(DeltaTime);
}