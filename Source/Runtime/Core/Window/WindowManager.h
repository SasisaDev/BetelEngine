#pragma once

#include "Window.h"
#include <queue>

class WindowManager
{
    std::vector<Window*> Windows;
    std::queue<int> DestroyPendingWindows;
    SDL_Event event;

    bool bShouldTerminate = false;

    Window* internal_GetBySDLWindow(SDL_Window* window);
public:

    Window* CreateWindow(WindowCreateInfo& createInfo);
    Window* GetWindow(window_t id);

    void CleanupClosePending();

    inline bool ShouldTerminate() const {return bShouldTerminate;}

    void Update(float deltaTime);

    /*
     * SDL Event Handler
     * Used for global SDL events, untied to windows
    */
    static MulticastDelegate<SDL_Event> OnSDLEvent;
};