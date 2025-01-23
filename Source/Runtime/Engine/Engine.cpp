#include "Engine.h"
#include <Assets/AssetLibrary.h>
#include <Platform/Platform.h>
#include <UI/Widgets/Canvas/Canvas.h>
#include <World/World.h>

Engine* GEngine = nullptr;

void Engine::HandleIncomingInputEvent(InputEvent &event)
{
    // Should not dispatch any events if the game is out of focus
    if(!GetGameFocused()) {
        return;
    }

    // TODO: Remove this
    /*if(event.KeyName.size() > 0)
        LOGF(Log, LogInput, "Key down: %s", event.KeyName.c_str());
    if(event.IsMouse && event.IsKey)
        LOGF(Log, LogInput, "Mouse button: %d", event.MouseButton);
    */
    if(world != nullptr)
    {
        world->PushInputEvent(event);
    }
}

Engine::Engine()
{
    GEngine = this;

    tickManager = new TickableManager();
    timerManager = new TimerManager();

    // Initialize i18n
    textManager = new TextManager();
    textManager->Initialize();

    // Subscribe to input events
    inputManager = new InputManager();
    inputManager->OnInputEvent.BindMember(this, &Engine::HandleIncomingInputEvent);

    canvasWidget = std::make_shared<CanvasWidget>();

    // Initialize Object Library
    objectLibrary = new ObjectLibrary;

    // Crawl all assets
    assetLibrary = new AssetLibrary();
    AssetLibrary::Get().CrawlAssetsAll("./Content");

    assetLoader = new AssetLoader();
    assetLoader->CrawlContent();

    GC = new AssetGarbageCollector();

}

Engine::~Engine()
{
    // Set GEngine to equall nullptr immediately
    // To prevent other classes from attemping using it while in closing state 
    GEngine = nullptr;

    world = nullptr;

    delete GC;
    GC = nullptr;
    
    delete assetLoader;
    assetLoader = nullptr;
    
    delete assetLibrary;
    assetLibrary = nullptr;
    
    delete objectLibrary;
    objectLibrary = nullptr;
    
    delete inputManager;
    inputManager = nullptr;
    
    delete textManager;
    textManager = nullptr;
    
    delete timerManager;
    timerManager = nullptr;
    
    delete tickManager;
    tickManager = nullptr;
}

void Engine::SetWorld(World* nWorld)
{
    world = nWorld;
#   ifndef EDITOR
    world->BeginPlay();
#   endif
    EngineDelegates::OnWorldLoad.Broadcast(world);
}

void Engine::LoadWorld(uint32_t worldID)
{
    LOGF(Log, LogEngine, "Trying to load world: 0x%08X.", worldID);

    if(!objectLibrary->IsObjectValid(worldID))
    {
        LOG(Error, LogEngine, "LoadWorld failed: passed World Reference is invalid.");
        return;
    }

    if(objectLibrary->LoadObject(worldID) == nullptr)
    {
        LOG(Error, LogEngine, "LoadWorld failed: passed World Reference is seemingly valid, but loading has failed.");
        return;
    }

    // TODO World Loading
}

void Engine::Tick(float DeltaTime)
{
    tickManager->Tick(DeltaTime);
    timerManager->Tick(DeltaTime);

    world->Tick(DeltaTime);

    canvasWidget->Tick(DeltaTime);
    canvasWidget->RecalculateChildrenTransforms();
}