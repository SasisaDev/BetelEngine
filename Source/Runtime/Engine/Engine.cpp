#include "Engine.h"
#include <Assets/AssetLibrary.h>
#include <Platform/Platform.h>

Engine::Engine()
{
    // Create default world
    world = new World;

    // Crawl all assets
    for(std::string& domain : IPlatform::Get()->GetLocalDomains()) {
        AssetLibrary::Get().CrawlAssetsAll(domain);
    }
}

void Engine::SetWorld(World* nWorld)
{
    world = nWorld;
    EngineDelegates::OnWorldLoad.Broadcast(world);
}

void Engine::Tick(float DeltaTime)
{
    tickManager.Tick(DeltaTime);

    world->Tick(DeltaTime);
}