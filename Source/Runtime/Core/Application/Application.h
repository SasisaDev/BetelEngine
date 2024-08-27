#pragma once

#include <cstdint>
#include <vector>
#include <queue>

#include "../Window/Window.h"
#include "Arguments.h"
#include <RenderV/Render.h>
#include <Engine/Engine.h>
#include <Settings/SettingsManager.h>

class Application
{
private:
    std::queue<int> DestroyPendingWindows;

    ArgumentCollection Arguments;
    std::string ApplicationPath;
protected:
    std::vector<Window*> Windows;
    
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

    inline bool ShouldTerminate() const {return bShouldTerminate;}

    window_t CreateWindow(WindowCreateInfo& createInfo);

    void ApplicationLoop();
};

extern Application* GApplication;