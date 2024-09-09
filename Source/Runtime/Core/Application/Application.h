#pragma once

#include <cstdint>
#include <vector>
#include <queue>

#include "../Window/WindowManager.h"
#include "Arguments.h"
#include <RenderV/Render.h>
#include <Engine/Engine.h>
#include <Settings/SettingsManager.h>

class Application
{
private:
    ArgumentCollection Arguments;
    std::string ApplicationPath;
protected:
    WindowManager* Windows;
    IRenderEngine* Render;
    Engine* GameEngine;
    SettingsManager* Settings;

    bool bShouldTerminate = false;
public:
    Application();
    Application(int argc, char* argv[]);
    ~Application();

    void Initialize();

    inline IRenderEngine* GetRender() const {return Render;}

    inline Engine* GetEngine() const {return GameEngine;}

    inline SettingsManager* GetSettings() const {return Settings;}

    inline bool ShouldTerminate() const {return bShouldTerminate || Windows->ShouldTerminate();}

    window_t CreateWindow(WindowCreateInfo& createInfo);

    void ApplicationLoop();

protected:
    void OnWindowEvent(Window* win, WindowEventPayload* payload);
};

extern Application* GApplication;