#pragma once

#include <string>
#include <cstdint>

#include <SDL.h>
#include <SDL_vulkan.h>

#include <RenderV/LayerRef.h>

#define DEIRI_MAX_WINDOWS 256

#if (DEIRI_MAX_WINDOWS > 4294967296)
#   error Maximum Window Amount is too huge!
#elif (DEIRI_MAX_WINDOWS > 65536)
    typedef uint32_t window_t;
#elif (DEIRI_MAX_WINDOWS > 256)
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
    SDL_Window* window;
	SDL_Surface* surface;
	SDL_Event event;

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

    void Update();
};