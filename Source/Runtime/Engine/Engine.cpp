#include "Engine.h"
#include <Assets/AssetLibrary.h>

Engine::Engine()
{
    // Create default world
    world = new World;

    // Crawl all assets
    AssetLibrary::Get().CrawlAssetsAll();
}

void Engine::Tick(float DeltaTime)
{
    tickManager.Tick(DeltaTime);

    world->Tick(DeltaTime);
}