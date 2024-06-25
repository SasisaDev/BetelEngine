#include "Application.h"

#include <ctime>
#include <chrono>

#include <Log/Logger.h>

Application* GApplication = nullptr;

Application::Application()
{
	if(GApplication == nullptr)
	{
		GApplication = this;
	}

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		LOG(Fatal, SDL, "SDL Initialization failed");
	}

	// Boilerplate for GetInstanceExtensions, until window handle is dropped in new SDL 
	SDL_Window* window;
	window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1, 1, SDL_WINDOW_VULKAN);

	// Create Render Engine object
    Render = IRenderFactory::Get().CreateEngine();

	// Get vulkan instance extensions from SDL
	unsigned int pExtCount = 0;

	SDL_Vulkan_GetInstanceExtensions(window, &pExtCount, 0);

	std::vector<const char*> SDLExtensions(pExtCount);

	if (SDL_Vulkan_GetInstanceExtensions(window, &pExtCount, SDLExtensions.data()) == SDL_FALSE)
	{
		LOG(Fatal, LogSDL, "Failed at aquiring instance extensions for Vulkan from SDL");
	}

	SDL_DestroyWindow(window);
	
	Render->Initialize(SDLExtensions);
	
}

Application::~Application()
{
	delete Render;
	Render = nullptr;
}

window_t Application::CreateWindow(WindowCreateInfo& createInfo)
{
	Window* window = new Window(createInfo);

	// Register Composition
	window->RendererCompositionID = Render->CreateComposition();
	IRenderComposition* composition = Render->GetComposition(window->RendererCompositionID);

	// Initialize Composition with Surface
	RenderCompositionInitializerSurface surfaceInitializer;
	window->CreateSurface(Render->GetInstance(), &surfaceInitializer.surface);

	composition->Initialize(&surfaceInitializer);

	// Add requested Layer Refs to Composition
	composition->AddLayerRefs(createInfo.layerRefs);

	Windows.push_back(window);
	return Windows.size() - 1;
}

void Application::ApplicationLoop()
{
	auto frame_lifetime_start = std::chrono::high_resolution_clock::now();
	auto frame_lifetime_end = std::chrono::high_resolution_clock::now();
	while(!bShouldTerminate)
	{
		float deltaTime = std::chrono::duration<float, std::milli>(frame_lifetime_end-frame_lifetime_start).count() / 1000;
		frame_lifetime_start = std::chrono::high_resolution_clock::now();

		//LOGF(Log, LogTime, "%f", deltaTime);
		for (int i = 0; i < Windows.size(); i++)
		{
			Windows[i]->Update();
		}

		Render->Render();

		frame_lifetime_end = std::chrono::high_resolution_clock::now();
	}
}