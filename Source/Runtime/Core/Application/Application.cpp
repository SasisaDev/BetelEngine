#include "Application.h"

#include <ctime>
#include <chrono>
#include <type_traits>
#include <thread>

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
	std::vector<std::string> args(argc);
	for(int i = 0; i < argc; ++i) {
		args[i] = argv[i];
	}
	IPlatform::Get()->SetExecVariables(args);

	ApplicationPath = IPlatform::Get()->GetExecutablePath();

	Arguments = ArgumentParser::ParseArgs(argc - 1, argv + 1);

	for(ArgumentEvent* event : ArgumentsEventHandler::Get().events)
	{
		if(!event->Handle(Arguments)) {
			exit(0);
			return;
		}
	}

	Initialize();
}

void Application::Initialize()
{
	if(GApplication != nullptr) {
		return;
	}
	
	GApplication = this;

	ApplicationPath = IPlatform::Get()->GetExecutablePath();

	// Create Virtual Filesystem handler
	if(IsEditor()) {
		IPlatform::Get()->AddLocalPath("Editor", IPath(IPlatform::Get()->GetExecutablePath()).StepBack());
		if(Arguments.Has("project")) {
			IPlatform::Get()->AddLocalPath("Game", Arguments.Get("project"));
		}
	} else {
		IPlatform::Get()->AddLocalPath("Game", IPath(IPlatform::Get()->GetExecutablePath()).StepBack());
		IPlatform::Get()->AddLocalPath("Editor", IPath(IPlatform::Get()->GetExecutablePath()).StepBack());
	}
	

	Windows = new WindowManager();

	// Create Settings Manager
	Settings = new SettingsManager();

	// Create Engine object
	GameEngine = new Engine();

	// Initialize SDL framework
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		LOG(Fatal, SDL, "SDL Initialization failed");
	}

	// Boilerplate for GetInstanceExtensions, until window handle is dropped in new SDL 
	SDL_Window* window;
	window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1, 1, SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN);

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

	std::vector<const char*> InstanceLayers;

	// TO DO: Refactor extensions
#	if !defined(NDEBUG)
	InstanceLayers.push_back("VK_LAYER_KHRONOS_validation");
	SDLExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#	endif
	
	Render->Initialize(SDLExtensions, InstanceLayers);

	VkSurfaceKHR dummySurface;
	SDL_Vulkan_CreateSurface(window, Render->GetInstance(), &dummySurface);

	Render->UpdateQueueFamilies(dummySurface);

	SDL_DestroyWindow(window);

#	if !defined(NDEBUG)
	Render->CreateDebugMessenger(DebugMessageCallback, 0);
#	endif

	LOG(Log, LogBetel, "Successfully initialized Application's Essentials");
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
	Window* window = Windows->CreateWindow(createInfo);

	// Subscribe to events
	window->OnWindowEvent.BindMember(this, &Application::OnWindowEvent);

	// Register Composition and
	// Initialize Composition with Surface
	RenderCompositionInitializerSurface surfaceInitializer;
	surfaceInitializer.extent = {createInfo.width, createInfo.height};
	surfaceInitializer.layerRefs = createInfo.layerRefs;
	window->CreateSurface(Render->GetInstance(), &surfaceInitializer.surface);

	window->RendererCompositionID = Render->CreateComposition(&surfaceInitializer);
	IRenderComposition* composition = Render->GetComposition(window->RendererCompositionID);

	LOGF(Log, LogWindow, "Successfully created window: %u", window->WindowID);

	return window->WindowID;
}

void Application::ApplicationLoop()
{
	auto frame_lifetime_start = std::chrono::high_resolution_clock::now();
	auto frame_lifetime_end = std::chrono::high_resolution_clock::now();

	float MinimalDeltaTime = 1000.f/120.f;

	while(!Windows->ShouldTerminate())
	{
		float deltaTime = std::chrono::duration<float, std::milli>(frame_lifetime_end-frame_lifetime_start).count() / 1000;
		frame_lifetime_start = std::chrono::high_resolution_clock::now();

		if (deltaTime < MinimalDeltaTime)
        {
            std::chrono::duration<double, std::milli> delta_ms(MinimalDeltaTime - deltaTime);
            auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
            std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
        }

		//LOGF(Log, LogTime, "%f", deltaTime);
		// Update all windows, may require compositions recreation
		Windows->Update(deltaTime);

		// Tick Game Engine
		GameEngine->Tick(deltaTime);

		// Perform rendering
		Render->Render();

		// Cleanup ShouldClose-Windows
		Windows->CleanupClosePending();

		frame_lifetime_end = std::chrono::high_resolution_clock::now();
	}
}

void Application::OnWindowEvent(Window* win, WindowEventPayload* payload)
{
	if(!win || !payload) {
		assert(!"Window Event callback must have window and payload pointer specified!");
		return;
	}

	bool recreateComp = false;
	RenderCompositionInitializerSurface reinitInfo;

	switch(payload->type) {
		case WINDOW_EVENT_RESIZE:
			recreateComp = true;
			reinitInfo.extent.width = ((WindowEventPayloadResize*)payload)->width;
			reinitInfo.extent.height =  ((WindowEventPayloadResize*)payload)->height;
			break;
	};

	if(recreateComp) {
		Render->GetComposition(win->RendererCompositionID)->Recreate(&reinitInfo);
	}
}