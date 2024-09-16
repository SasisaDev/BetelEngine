#pragma once

#include "EngineDelegates.h"
#include "Timer.h"
#include "Tickable.h"
#include <Assets/AssetLibrary.h>
#include <Assets/AssetSmartPointers.h>
#include <World/World.h>
#include <UI/Widget.h>

class Engine
{
protected:
    TickableManager tickManager;
    TimerManager timerManager;
    AssetLibrary assetLibrary;

    World* world;

    std::shared_ptr<Widget> canvasWidget;
#	ifdef EDITOR
    std::shared_ptr<Widget> edCanvasWidget;
#   endif
public:
    Engine();

    TickableManager& GetTickManager() {return tickManager;}
    TimerManager& GetTimer() {return timerManager;}
    AssetLibrary& GetAssetLibrary() {return assetLibrary;}

#pragma region "World API"
    // Shouldn't be used, instead use Engine::LoadWorld
    void SetWorld(World* world);
    void LoadWorld(AssetPtr<World> worldPtr);
    inline World* GetWorld() const {return world;}
#pragma endregion "World API"

#pragma region "UI API"
    void SetCanvasWidget(std::shared_ptr<Widget> newCanvasWidget) {canvasWidget = newCanvasWidget;}
    inline Widget* GetCanvasWidget() const {return canvasWidget.get();}
#	ifdef EDITOR
    void SetEditorCanvasWidget(std::shared_ptr<Widget> newCanvasWidget) {edCanvasWidget = newCanvasWidget;}
    inline Widget* GetEditorCanvasWidget() const {return edCanvasWidget.get();}
#   endif
#pragma endregion "UI API"

    void Tick(float DeltaTime);
};

extern Engine* GEngine;