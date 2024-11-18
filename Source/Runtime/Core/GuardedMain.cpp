#include <Platform/Platform.h>
#include <Core/Application/Application.h>
#include <World/RenderLayer/WorldRenderLayer.h>
#include <UI/RenderLayer/WidgetRenderLayer.h>
#include <Log/Logger.h>

#include <Settings/SettingsManager.h>
#include <GameFramework/Settings/GameSettings.h>

#include <RenderV/Objects/Buffers/Buffer.h>

#ifdef EDITOR
#	include <ImGui/RenderLayer/ImGuiLayer.h>
#	include <EditorUI/EditorToolkit.h> 
#	include <ToolkitRenderLayer/ToolkitRenderLayer.h>
#endif

// TODO: Remove test boilerplate
#include <GameFramework/Entities/Test/TestEntity.h>
#include <UI/Widget.h>
#include <Assets/AssetFile/AssetFile.h>
#include <UI/Widgets/Panel/Panel.h>

int GuardedMain(int argc, char* argv[])
{
	LOG(Log, LogStartup, "BetelEngine::GuardedMain()");

	Application app(argc, argv);

	IRenderEngine* render = app.GetRender();

	render->CreateDevice();

	// Add render layers

	render->CreateLayer<WorldRenderLayer>();
	render->CreateLayer<UIRenderLayer>();
#ifdef EDITOR
	render->CreateLayer<ToolkitRenderLayer>();
	render->CreateLayer<ImGuiRenderLayer>();
#endif
	// Fetch settings
	GameSettings* settings = app.GetSettings()->GetOrDefault<GameSettings>();

	// Create game window
#ifdef EDITOR
	std::vector<IRenderLayerRef*> editorCompositionLayerRefs = {
		IRenderFactory::CreateLayerRef<WorldRenderLayer, WorldRenderLayerRef>(render)
			->SetViewportSize({settings->PixelPerfectViewportWidth, settings->PixelPerfectViewportHeight})
			->SubscribeWorldLoad(&EngineDelegates::OnWorldLoad)
			->SubscribeWorldUnload(&EngineDelegates::OnWorldUnload),
		IRenderFactory::CreateLayerRef<UIRenderLayer, UIRenderLayerRef>(render)->SetCanvasWidget(app.GetEngine()->GetCanvasWidget()),
		IRenderFactory::CreateLayerRef<ToolkitRenderLayer, ToolkitRenderLayerRef>(render),
		IRenderFactory::CreateLayerRef<ImGuiRenderLayer, ImGuiRenderLayerRef>(render)
			->SetToolkit(new EditorToolkitBase)
	};

	WindowCreateInfo editorWininfo = {
		.title="Betel Editor | " + settings->GameTitle,
		.width=1280,
		.height=720,
		.layerRefs=editorCompositionLayerRefs
	};

	window_t edWinId = app.CreateWindow(editorWininfo);
	// TODO: More robust host window setup for ImGui Layer
	((ImGuiRenderLayerRef*)editorCompositionLayerRefs[3])->SetHostWindow(app.GetWindow(edWinId));
#else
	std::vector<IRenderLayerRef*> gameCompositionLayerRefs = {
		IRenderFactory::CreateLayerRef<WorldRenderLayer, WorldRenderLayerRef>(render)
			->SetViewportSize({settings->PixelPerfectViewportWidth, settings->PixelPerfectViewportHeight})
			->SubscribeWorldLoad(&EngineDelegates::OnWorldLoad)
			->SubscribeWorldLoad(&EngineDelegates::OnWorldUnload),
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

	// TODO: Delete these synthetic calls
	EngineDelegates::OnWorldLoad.Broadcast(app.GetEngine()->GetWorld());
	app.GetEngine()->GetWorld()->SetBackgroundColor(Vec3(0.75, 0.5, 0));
	app.GetEngine()->GetWorld()->Spawn<EntityTest>("TestEntity", EntitySpawnInfo());

	app.GetEngine()->GetCanvasWidget()->AddChild(std::make_shared<PanelWidget>());

	IDirectory* directory = IPlatform::Get()->OpenDirectory("./Content", DIRECTORY_FLAG_RECURSIVE);

	/*AssetFile assetFile("Editor/Content/AssetFile");
	assetFile.ReadFromDevice();*/

	//AssetLibrary::Get().LoadAsset("Editor/Content/Shaders/WidgetBrushDefault");

	app.ApplicationLoop();

	return 0;
}