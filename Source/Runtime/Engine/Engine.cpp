#include "Engine.h"
#include <Assets/AssetLibrary.h>
#include <Platform/Platform.h>
#include <UI/Widgets/Canvas/Canvas.h>

Engine* GEngine = nullptr;

Engine::Engine()
{
    GEngine = this;

    // Initialize i18n
    textManager.Initialize();

    // Create default world
    world = new World;
    canvasWidget = std::make_shared<CanvasWidget>();

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
    canvasWidget->RecalculateChildrenTransforms();
}