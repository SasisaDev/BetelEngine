#include "WindowManager.h"

Window* WindowManager::internal_GetBySDLWindow(SDL_Window* window)
{
    for(Window* win : Windows)
    {
        if(win && win->window == window)
        {
            return win;
        }
    }
    return nullptr;
}

Window* WindowManager::CreateWindow(WindowCreateInfo& createInfo)
{
    Window* window = new Window(createInfo);

    Windows.push_back(window);
    window->WindowID = Windows.size() - 1;

    return window;
}

Window* WindowManager::GetWindow(window_t id)
{
    for(Window* win : Windows)
    {
        if(win && win->WindowID == id)
        {
            return win;
        }
    }
    return nullptr;
}

void WindowManager::Update(float deltaTime)
{
    SDL_PollEvent(&event);

    Window* emittedWindow = nullptr;
	
	switch(event.type)
	{
		/*case SDL_QUIT:
			bShouldClose = true;
			break;*/
		case SDL_WINDOWEVENT:
			// Handle window events
            emittedWindow = internal_GetBySDLWindow(SDL_GetWindowFromID(event.window.windowID));
			if(emittedWindow) {
				emittedWindow->Update(event.window);
			}
			break;
	}
}

void WindowManager::CleanupClosePending()
{
    for(int i = 0; i < Windows.size(); i++)
		{
			if(Windows[i]->bShouldClose)
			{
				DestroyPendingWindows.push(i);
			}
		}

		// Destroy windows that should close
		for(; !DestroyPendingWindows.empty(); DestroyPendingWindows.pop())
		{
			// Kill window
			delete Windows[DestroyPendingWindows.front()];
			Windows.erase(Windows.begin() += DestroyPendingWindows.front());

			// If we're out of windows, close application
			if(Windows.size() <= 0)
			{
				bShouldTerminate = true;
			}
		}
}