#include "Engine.h"
#include <Assets/AssetLibrary.h>
#include <Platform/Platform.h>
#include <UI/Widgets/Canvas/Canvas.h>

Engine* GEngine = nullptr;

void Engine::HandleIncomingInputEvent(InputEvent event)
{
    // Should not dispatch any events if the game is out of focus
    if(!GetGameFocused()) {
        return;
    }

    if(event.KeyName.size() > 0)
        LOGF(Log, LogInput, "Key down: %s", event.KeyName.c_str());

    // TODO: Forward Event to the World

}

Engine::Engine()
{
    GEngine = this;

    // Initialize i18n
    textManager.Initialize();

    // Subscribe to input events
    inputManager.OnInputEvent.BindMember(this, &Engine::HandleIncomingInputEvent);

    // Create default world
    world = new World;
    canvasWidget = std::make_shared<CanvasWidget>();

    // Crawl all assets
    AssetLibrary::Get().CrawlAssetsAll("./Content");
}

void Engine::SetWorld(World* nWorld)
{
    world = nWorld;
#   ifndef EDITOR
    world->BeginPlay();
#   endif
    EngineDelegates::OnWorldLoad.Broadcast(world);
}

void Engine::LoadWorld(ObjectRef<World> worldRef)
{
    LOGF(Log, LogEngine, "Trying to load world: 0x%08X.", worldRef.GetID());

    if(!worldRef.IsValid())
    {
        LOG(Error, LogEngine, "LoadWorld failed: passed World Reference is invalid.");
        return;
    }

    if(worldRef.Load() == nullptr)
    {
        LOG(Error, LogEngine, "LoadWorld failed: passed World Reference is seemingly valid, but loading has failed.");
        return;
    }

    // TODO World Loading
}

void Engine::Tick(float DeltaTime)
{
    tickManager.Tick(DeltaTime);
    timerManager.Tick(DeltaTime);

    world->Tick(DeltaTime);

    canvasWidget->Tick(DeltaTime);
    canvasWidget->RecalculateChildrenTransforms();
}