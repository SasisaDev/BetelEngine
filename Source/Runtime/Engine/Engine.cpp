#include "Engine.h"
#include <Assets/AssetLibrary.h>
#include <Platform/Platform.h>
#include <UI/Widgets/Canvas/Canvas.h>

Engine::Engine()
{
    // Create default world
    world = new World;
    canvasWidget = std::make_shared<CanvasWidget>();
#ifdef EDITOR
    // TODO: Toolkit auto-builder
    edCanvasWidget = std::make_shared<CanvasWidget>();
#endif

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

    canvasWidget->Tick(DeltaTime);
}