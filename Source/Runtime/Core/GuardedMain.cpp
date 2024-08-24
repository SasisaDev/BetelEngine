#pragma once

#include <Core/Application/Application.h>
#include <World/RenderLayer/WorldRenderLayer.h>
#include <UI/RenderLayer/WidgetRenderLayer.h>
#include <Log/Logger.h>

#include <RenderV/Objects/Buffers/Buffer.h>

int GuardedMain(int argc, char* argv[])
{
	LOG(Log, LogStartup, "ArteriaFrameworks::GuardedMain()");

	Application app(argc, argv);

	IRenderEngine* render = app.GetRender();

	render->CreateDevice();

	// Add render layers

	render->CreateLayer<WorldRenderLayer>();
	render->CreateLayer<UIRenderLayer>();

	// Create game window
#ifdef EDITOR
	std::vector<IRenderLayerRef*> editorCompositionLayerRefs = {
		IRenderFactory::CreateLayerRef<UIRenderLayer, UIRenderLayerRef>(render)->SetCanvasWidget(app.GetEngine()->GetEditorCanvasWidget()),
		IRenderFactory::CreateLayerRef<WorldRenderLayer, WorldRenderLayerRef>(render),
		IRenderFactory::CreateLayerRef<UIRenderLayer, UIRenderLayerRef>(render)->SetCanvasWidget(app.GetEngine()->GetCanvasWidget()),
	};

	WindowCreateInfo editorWininfo = {
		.title="Arteria Editor",
		.width=1280,
		.height=720,
		.layerRefs=editorCompositionLayerRefs
	};

	app.CreateWindow(editorWininfo);
#else
	std::vector<IRenderLayerRef*> gameCompositionLayerRefs = {
		IRenderFactory::CreateLayerRef<WorldRenderLayer, WorldRenderLayerRef>(render),
		IRenderFactory::CreateLayerRef<UIRenderLayer, UIRenderLayerRef>(render)->SetCanvasWidget(app.GetEngine()->GetCanvasWidget()),
	};

	WindowCreateInfo gameWininfo = {
		.title="Game",
		.width=1280,
		.height=720,
		.layerRefs=gameCompositionLayerRefs
	};
	
	app.CreateWindow(gameWininfo);
#endif

	volatile Buffer<unsigned long long[5]> buff(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

	app.ApplicationLoop();

	return 0;
}