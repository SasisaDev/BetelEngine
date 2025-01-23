#pragma once

#include "EngineDelegates.h"
#include "Tickable.h"
#include <Assets/AssetLibrary.h>
#include <Input/InputManager.h>
#include <i18n/TextManager.h>
#include <AssetLoader/GC/AssetGarbageCollector.h>
#include <Timer/Timer.h>
#include <Object/ObjectLibrary.h>
#include <AssetLoader/AssetLoader.h>

class World;
class Widget;

class Engine
{
    virtual void HandleIncomingInputEvent(InputEvent &event);
protected:
    TickableManager* tickManager;
    TimerManager* timerManager;
    AssetLibrary* assetLibrary;
    InputManager* inputManager;
    TextManager* textManager;
    AssetGarbageCollector* GC;
    ObjectLibrary* objectLibrary;
    AssetLoader* assetLoader;

    World* world = nullptr;

    std::shared_ptr<Widget> canvasWidget;

    bool IsGameFocused = true;
public:
    Engine();
    ~Engine();

    TickableManager* GetTickManager() {return tickManager;}
    TimerManager* GetTimer() {return timerManager;}
    AssetLibrary* GetAssetLibrary() {return assetLibrary;}
    InputManager* GetInputManager() {return inputManager;}
    TextManager* GetTextManager() {return textManager;}
    ObjectLibrary* GetObjectLibrary() {return objectLibrary;}
    AssetLoader* GetAssetLoader() {return assetLoader;}

    void SetGameFocused(bool bIsFocused) {IsGameFocused = bIsFocused;} 
    bool GetGameFocused() {return IsGameFocused;}

#pragma region "World API"
    // Shouldn't be used, instead use Engine::LoadWorld
    void SetWorld(World* world);
    void LoadWorld(uint32_t ID);
    inline World* GetWorld() const {return world;}
#pragma endregion "World API"

#pragma region "UI API"
    void SetCanvasWidget(std::shared_ptr<Widget> newCanvasWidget) {canvasWidget = newCanvasWidget;}
    inline Widget* GetCanvasWidget() const {return canvasWidget.get();}
#pragma endregion "UI API"

    void Tick(float DeltaTime);
};

extern Engine* GEngine;