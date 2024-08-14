#pragma once

#include <Core/Application/Application.h>
#include <World/RenderLayer/WorldRenderLayer.h>
#include <Log/Logger.h>

int GuardedMain()
{
	LOG(Log, LogStartup, "ArteriaFrameworks::GuardedMain()");

	Application app;

	IRenderEngine* render = app.GetRender();

	// Add render layers

	render->CreateLayer<WorldRenderLayer>();

	// Create game window
	std::vector<IRenderLayerRef*> gameCompositionLayerRefs = {
		IRenderFactory::CreateLayerRef<WorldRenderLayer, WorldRenderLayerRef>(render)
	};

	WindowCreateInfo wininfo = {
		.title="Test",
		.width=1280,
		.height=720,
		.layerRefs=gameCompositionLayerRefs
	};
	app.CreateWindow(wininfo);

	// Creating device after creation of Window to include present queue checks
	render->CreateDevice();

	app.ApplicationLoop();

	return 0;
}