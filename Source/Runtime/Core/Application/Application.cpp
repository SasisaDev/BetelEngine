#include "Application.h"

#include <ctime>
#include <chrono>
#include <type_traits>

#include <Log/Logger.h>
#include <Platform/Platform.h>

Application* GApplication = nullptr;

#if !defined(NDEBUG)
VkBool32 __stdcall DebugMessageCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
    void*                                            pUserData)
{
	IPlatform::Get()->DebugPrint((std::string(pCallbackData->pMessage) + "\n").c_str());
	return VK_FALSE;
}
#endif

Application::Application()
{
	Initialize();	
}

Application::Application(int argc, char* argv[])
{
	Arguments = ArgumentsParser::ParseArgs(argc, argv);

	Initialize();
}

void Application::Initialize()
{
	if(GApplication != nullptr)
	{
		return;
	}
	
	GApplication = this;

	// Create Engine object
	GameEngine = new Engine();

	// Initialize SDL framework
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		LOG(Fatal, SDL, "SDL Initialization failed");
	}

	// Boilerplate for GetInstanceExtensions, until window handle is dropped in new SDL 
	SDL_Window* window;
	window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1, 1, SDL_WINDOW_VULKAN);

	// Create Render Engine object
    Render = IRenderFactory::CreateEngine();

	// Get vulkan instance extensions from SDL
	unsigned int pExtCount = 0;

	SDL_Vulkan_GetInstanceExtensions(window, &pExtCount, 0);

	std::vector<const char*> SDLExtensions(pExtCount);

	if (SDL_Vulkan_GetInstanceExtensions(window, &pExtCount, SDLExtensions.data()) == SDL_FALSE)
	{
		LOG(Fatal, LogSDL, "Failed at aquiring instance extensions for Vulkan from SDL");
	}

	SDL_DestroyWindow(window);

	std::vector<const char*> InstanceLayers;

	// TO DO: Refactor extensions
#	if !defined(NDEBUG)
	InstanceLayers.push_back("VK_LAYER_KHRONOS_validation");
	SDLExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#	endif
	
	Render->Initialize(SDLExtensions, InstanceLayers);

#	if !defined(NDEBUG)
	Render->CreateDebugMessenger(DebugMessageCallback, 0);
#	endif
}

Application::~Application()
{
	delete Render;
	Render = nullptr;

	delete GameEngine;
	GameEngine = nullptr;
}

window_t Application::CreateWindow(WindowCreateInfo& createInfo)
{
	Window* window = new Window(createInfo);

	// Register Composition and
	// Initialize Composition with Surface
	RenderCompositionInitializerSurface surfaceInitializer;
	window->CreateSurface(Render->GetInstance(), &surfaceInitializer.surface);

	window->RendererCompositionID = Render->CreateComposition(&surfaceInitializer);
	IRenderComposition* composition = Render->GetComposition(window->RendererCompositionID);

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
		// Update all windows, may require compositions recreation
		for (int i = 0; i < Windows.size(); i++)
		{
			Windows[i]->Update();
		}

		// Tick Game Engine
		GameEngine->Tick(deltaTime);

		// Perform rendering
		Render->Render();

		// Check windows state
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

		frame_lifetime_end = std::chrono::high_resolution_clock::now();
	}
}