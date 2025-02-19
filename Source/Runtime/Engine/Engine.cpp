#include "Engine.h"
#include <Assets/AssetLibrary.h>
#include <Platform/Platform.h>
#include <UI/Widgets/Canvas/Canvas.h>
#include <World/World.h>

#if !defined(NDEBUG) && !defined(EDITOR)
#   include <Debug/Debug.h>
#endif

Engine* GEngine = nullptr;

void Engine::HandleIncomingInputEvent(InputEvent &event)
{
    // Should not dispatch any events if the game is out of focus
    if(!GetGameFocused()) {
        return;
    }

    // Debug
#   if !defined(NDEBUG) && !defined(EDITOR)
    if(event.KeyName == "`" && !event.IsUp)
    {
        LOG(Log, LogDebug, "Toggle debug menu");
        DebugImGui::Get()->Toggle();
    }
#   endif

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

void Engine::InternalTravel()
{
    // TODO: Unload old world
    if(world){
        EngineDelegates::OnWorldUnload.Broadcast(world);
        //objectLibrary->UnloadObject(world->GetID());
    }

    world = travelWorld;
    world->SpawnDefaultEntities();
    world->PostInit();

#   ifndef EDITOR
    world->BeginPlay();
#   endif
    EngineDelegates::OnWorldLoad.Broadcast(world);

    LOGF(Log, LogEngine, "Traveled to world: 0x%08X.", world->GetID());

    travelWorld = nullptr;
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

    // Crawl all assets
    // TODO: Remove outdated code
    assetLibrary = new AssetLibrary();
    AssetLibrary::Get().CrawlAssetsAll("./Content");

    // Initialize AssetLoader
    assetLoader = new AssetLoader();

    // Initialize Object Library
    objectLibrary = new ObjectLibrary(assetLoader);

    GC = new AssetGarbageCollector(objectLibrary);

    // Crawl Content
    assetLoader->CrawlContent();
}

Engine::~Engine()
{
    // Set GEngine to equall nullptr immediately
    // To prevent other classes from attemping using it while in closing state 
    GEngine = nullptr;

    world = nullptr;

    delete GC;
    GC = nullptr;
    
    delete assetLibrary;
    assetLibrary = nullptr;
    
    delete objectLibrary;
    objectLibrary = nullptr;

    delete assetLoader;
    assetLoader = nullptr;
    
    delete inputManager;
    inputManager = nullptr;
    
    delete textManager;
    textManager = nullptr;
    
    delete timerManager;
    timerManager = nullptr;
    
    delete tickManager;
    tickManager = nullptr;
}

void Engine::TravelTo(World* nWorld)
{
    travelWorld = nWorld;
}

void Engine::LoadWorld(uint32_t worldID)
{
    if(!objectLibrary->IsObjectValid(worldID))
    {
        LOG(Error, LogEngine, "LoadWorld failed: passed World Reference is invalid.");
        return;
    }

    World* world = dynamic_cast<World*>(objectLibrary->LoadObject(worldID));
    if(world == nullptr)
    {
        LOG(Error, LogEngine, "LoadWorld failed: passed World Reference is seemingly valid, but loading has failed.");
        return;
    }

    TravelTo(world);

    LOGF(Log, LogEngine, "Loaded world: 0x%08X.", worldID);
}

void Engine::Tick(float DeltaTime)
{
    currentDeltaTime = DeltaTime;

    if(travelWorld) {
        InternalTravel();
    }

    // Tick world
    tickManager->Tick(DeltaTime);
    timerManager->Tick(DeltaTime);

    world->Tick(DeltaTime);

    canvasWidget->Tick(DeltaTime);
    canvasWidget->RecalculateChildrenTransforms();
}