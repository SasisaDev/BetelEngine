#include "Window.h"

#include <Log/Logger.h>

Window::Window(WindowCreateInfo& createInfo)
{
    title = createInfo.title;
    width = createInfo.width;
    height = createInfo.height;

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

	surface = SDL_GetWindowSurface(window);

	//SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0x00, 0xFF));

	SDL_UpdateWindowSurface(window);
}

void Window::CreateSurface(VkInstance instance, VkSurfaceKHR* vksurface)
{
    if(SDL_Vulkan_CreateSurface(window, instance, vksurface) == SDL_FALSE)
	{
		LOG(Fatal, LogSDL, "Failed creating vulkan surface out of window.");
	}
	LOGF(Log, LogWindow, "Created surface: %u", surface);
}

void Window::Update(const SDL_WindowEvent& event)
{
	switch(event.event)
	{
        case SDL_WINDOWEVENT_CLOSE:
            bShouldClose = true;
		case SDL_WINDOWEVENT_RESIZED:
            if(!bShouldClose) {
			    OnWindowEvent.Broadcast(this, new WindowEventPayloadResize(event.data1, event.data2));
            }
			break;
	}
}