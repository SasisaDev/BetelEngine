#pragma once

#include <string>
#include <cstdint>

#include <SDL.h>
#include <SDL_vulkan.h>

#include <RenderV/LayerRef.h>
#include <Delegate/Delegate.h>

#include "WindowEvents.h"

#define BETEL_MAX_WINDOWS 256

#if (BETEL_MAX_WINDOWS > 4294967296)
#   error Maximum Window Amount is too huge!
#elif (BETEL_MAX_WINDOWS > 65536)
    typedef uint32_t window_t;
#elif (BETEL_MAX_WINDOWS > 256)
    typedef uint16_t window_t;
#else 
    typedef uint8_t window_t;
#endif

struct WindowCreateInfo
{
    std::string title;
    uint32_t width, height;

    std::vector<IRenderLayerRef*> layerRefs;
};

class Window
{
    friend class WindowManager;
    
    SDL_Window* window;
	SDL_Surface* surface;

    std::string title;
    unsigned int width, height;

    bool bShouldClose = false;
protected:
    uint32_t RendererCompositionID;
    window_t WindowID;

    friend class Application;
public:
    Window(WindowCreateInfo& createInfo);

    void CreateSurface(VkInstance instance, VkSurfaceKHR* vksurface);

    void Update(const SDL_WindowEvent& event);

    /*
     * Window Event handler
     * Handles resize, minmax, repos scenarios
    */
    MulticastDelegate<Window*, WindowEventPayload*> OnWindowEvent;
};