#pragma once

#include <Platform/Platform.h>
#include <Core/Application/Application.h>
#include <World/RenderLayer/WorldRenderLayer.h>
#include <UI/RenderLayer/WidgetRenderLayer.h>
#include <Log/Logger.h>

#include <Settings/SettingsManager.h>
#include <GameFramework/Settings/GameSettings.h>

#include <RenderV/Objects/Buffers/Buffer.h>

#include <GameFramework/Entities/Test/TestEntity.h>

int GuardedMain(int argc, char* argv[])
{
	LOG(Log, LogStartup, "BetelEngine::GuardedMain()");

	Application app(argc, argv);

	IRenderEngine* render = app.GetRender();

	render->CreateDevice();

	// Add render layers

	render->CreateLayer<WorldRenderLayer>();
	render->CreateLayer<UIRenderLayer>();

	// Fetch settings
	GameSettings* settings = app.GetSettings()->GetOrDefault<GameSettings>();

	// Create game window
#ifdef EDITOR
	std::vector<IRenderLayerRef*> editorCompositionLayerRefs = {
		IRenderFactory::CreateLayerRef<WorldRenderLayer, WorldRenderLayerRef>(render)
			->SetViewportSize({settings->PixelPerfectViewportWidth, settings->PixelPerfectViewportHeight})
			->SetWorld(app.GetEngine()->GetWorld()),
		IRenderFactory::CreateLayerRef<UIRenderLayer, UIRenderLayerRef>(render)->SetCanvasWidget(app.GetEngine()->GetCanvasWidget()),
		IRenderFactory::CreateLayerRef<UIRenderLayer, UIRenderLayerRef>(render)->SetCanvasWidget(app.GetEngine()->GetEditorCanvasWidget()),
	};

	WindowCreateInfo editorWininfo = {
		.title="Betel Editor [" + settings->GameTitle + "]",
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
		.title=settings->GameTitle,
		.width=1280,
		.height=720,
		.layerRefs=gameCompositionLayerRefs
	};
	
	app.CreateWindow(gameWininfo);
#endif

	app.GetEngine()->GetWorld()->SetBackgroundColor(Vec3(0.75, 0.5, 0));
	app.GetEngine()->GetWorld()->Spawn<EntityTest>("TestEntity", EntitySpawnInfo());

	IDirectory* directory = IPlatform::Get()->OpenDirectory("./Content", DIRECTORY_FLAG_RECURSIVE);

	app.ApplicationLoop();

	return 0;
}