#pragma once

#include <Core/Application/Application.h>
#include <World/RenderLayer/WorldRenderLayer.h>
#include <UI/RenderLayer/WidgetRenderLayer.h>
#include <Log/Logger.h>

int GuardedMain()
{
	LOG(Log, LogStartup, "ArteriaFrameworks::GuardedMain()");

	Application app;

	IRenderEngine* render = app.GetRender();

	// Add render layers

	render->CreateLayer<WorldRenderLayer>();
	render->CreateLayer<UIRenderLayer>();

	// Create game window
	std::vector<IRenderLayerRef*> gameCompositionLayerRefs = {
#		ifdef EDITOR
		// Editor UI
		IRenderFactory::CreateLayerRef<UIRenderLayer, UIRenderLayerRef>(render)->SetCanvasWidget(app.GetEngine()->GetEditorCanvasWidget()),
#		endif
		IRenderFactory::CreateLayerRef<WorldRenderLayer, WorldRenderLayerRef>(render),
		IRenderFactory::CreateLayerRef<UIRenderLayer, UIRenderLayerRef>(render)->SetCanvasWidget(app.GetEngine()->GetCanvasWidget()),
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