#pragma once

#include <cstdint>
#include <vector>
#include <queue>

#include "../Window/Window.h"
#include <RenderV/Render.h>

class Application
{
private:
    std::queue<int> DestroyPendingWindows;
protected:
    std::vector<Window*> Windows;
    
    IRenderEngine* Render;

    bool bShouldTerminate = false;
public:
    Application();
    ~Application();

    inline IRenderEngine* GetRender() const {return Render;}

    inline bool ShouldTerminate() const {return bShouldTerminate;}

    window_t CreateWindow(WindowCreateInfo& createInfo);

    void ApplicationLoop();
};

extern Application* GApplication;