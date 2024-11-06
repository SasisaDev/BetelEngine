#pragma once

#include "EngineDelegates.h"
#include "Timer.h"
#include "Tickable.h"
#include <Assets/AssetLibrary.h>
#include <Assets/AssetSmartPointers.h>
#include <World/World.h>
#include <UI/Widget.h>
#include <Input/InputManager.h>

class Engine
{
protected:
    TickableManager tickManager;
    TimerManager timerManager;
    AssetLibrary assetLibrary;
    InputManager inputManager;

    World* world;

    std::shared_ptr<Widget> canvasWidget;

    bool IsGameFocused = true;
public:
    Engine();

    TickableManager& GetTickManager() {return tickManager;}
    TimerManager& GetTimer() {return timerManager;}
    AssetLibrary& GetAssetLibrary() {return assetLibrary;}
    InputManager& GetInputManager() {return inputManager;}

    void SetGameFocused(bool bIsFocused) {IsGameFocused = bIsFocused;} 
    bool GetGameFocused() {return IsGameFocused;}

#pragma region "World API"
    // Shouldn't be used, instead use Engine::LoadWorld
    void SetWorld(World* world);
    void LoadWorld(AssetPtr<World> worldPtr);
    inline World* GetWorld() const {return world;}
#pragma endregion "World API"

#pragma region "UI API"
    void SetCanvasWidget(std::shared_ptr<Widget> newCanvasWidget) {canvasWidget = newCanvasWidget;}
    inline Widget* GetCanvasWidget() const {return canvasWidget.get();}
#pragma endregion "UI API"

    void Tick(float DeltaTime);
};

extern Engine* GEngine;