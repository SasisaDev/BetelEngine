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
}

void Window::Update()
{
	// TODO: SDL Events are global and do not need polling for every window, except for WINDOWEVENT
	SDL_PollEvent(&event);
	
	switch(event.type)
	{
		case SDL_QUIT:
			bShouldClose = true;
			break;
		case SDL_WINDOWEVENT:
			// Handle window events
			switch(event.window.event)
			{
				case SDL_WINDOWEVENT_RESIZED:
					OnWindowEvent.Broadcast(this, new WindowEventPayloadResize(event.window.data1, event.window.data2));
					break;
			}
			break;
	}
}