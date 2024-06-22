#include "Application.h"

#include <Log/Logger.h>

Application::Application()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		LOG(Fatal, SDL, "SDL Initialization failed");
	}
}