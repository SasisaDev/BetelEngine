#pragma once

#include <Core/Application/Application.h>
#include <World/RenderLayer/WorldRenderLayer.h>
#include <Log/Logger.h>

int GuardedMain()
{
	LOG(Log, LogStartup, "DEIRI Frameworks");

	Application app;

	IRenderEngine* render = app.GetRender();

	// Add render layers

	render->CreateLayer<WorldRenderLayer>();

	// Create game window
	std::vector<IRenderLayerRef*> gameCompositionLayerRefs = {
		IRenderFactory::CreateLayerRef<WorldRenderLayer>(render)
	};

	WindowCreateInfo wininfo = {
		.title="Test",
		.width=1280,
		.height=720,
		.layerRefs=gameCompositionLayerRefs
	};
	app.CreateWindow(wininfo);

	app.ApplicationLoop();

	return 0;
}