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

    Widget* canvasWidget;
#	ifdef EDITOR
    Widget* edCanvasWidget;
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
    void SetCanvasWidget(Widget* newCanvasWidget) {canvasWidget = newCanvasWidget;}
    inline Widget* GetCanvasWidget() const {return canvasWidget;}
#	ifdef EDITOR
    void SetEditorCanvasWidget(Widget* newCanvasWidget) {edCanvasWidget = newCanvasWidget;}
    inline Widget* GetEditorCanvasWidget() const {return edCanvasWidget;}
#   endif
#pragma endregion "UI API"

    void Tick(float DeltaTime);
};

extern Engine* GEngine;