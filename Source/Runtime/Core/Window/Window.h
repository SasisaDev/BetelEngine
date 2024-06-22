#pragma once

#include <cstdint>

#include <SDL.h>
#include <SDL_Vulkan.h>

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

};

class Window
{
    SDL_Window* window;
	SDL_Surface* surface;
	SDL_Event event;
protected:
    uint32_t RendererCompositionID;
    window_t WindowID;
public:
    Window(WindowCreateInfo createInfo);
};