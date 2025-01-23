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
#	include <Editor/Editor.h>
#	include <ToolkitRenderLayer/ToolkitRenderLayer.h>
#endif

// TODO: Remove test boilerplate
#include <GameFramework/Entities/Test/TestEntity.h>
#include <UI/Widget.h>
#include <Assets/AssetFile/AssetFile.h>
#include <UI/Widgets/Panel/Panel.h>
#include <GameFramework/Entities/Sprite/SpriteEntity.h>
#include <World/Entities/Camera/Camera.h>
#include <AssetLoader/AssetLoader.h>
#include <GameFramework/Entities/Tilemap/Tilemap.h>
#include <World/World.h>
#include <GameFramework/Assets/Texture/Texture.h>

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
	ImGuiRenderLayerRef* imGuiLayer = nullptr;

	std::vector<IRenderLayerRef*> editorCompositionLayerRefs = {
		IRenderFactory::CreateLayerRef<WorldRenderLayer, WorldRenderLayerRef>(render)
			// TODO: This Viewport Size should ideally be equal to a window size
			->SetViewportSize({1280, 720})
			->SetCompensateAspectRatio(true)
			->SubscribeWorldLoad(&EngineDelegates::OnWorldLoad)
			->SubscribeWorldUnload(&EngineDelegates::OnWorldUnload),
		//IRenderFactory::CreateLayerRef<UIRenderLayer, UIRenderLayerRef>(render)->SetCanvasWidget(app.GetEngine()->GetCanvasWidget()),
		IRenderFactory::CreateLayerRef<ToolkitRenderLayer, ToolkitRenderLayerRef>(render),
		IRenderFactory::CreateLayerRef<ImGuiRenderLayer, ImGuiRenderLayerRef>(render)
			->SetToolkit(Editor::Get()->GetToolkitUI())
			->SetMaxSets(16)
			->PushInto(&imGuiLayer)
	};

	WindowCreateInfo editorWininfo = {
		.title="Betel Editor | " + settings->GameTitle,
		.width=1280,
		.height=720,
		.layerRefs=editorCompositionLayerRefs
	};

	window_t edWinId = app.CreateWindow(editorWininfo);
	imGuiLayer->SetHostWindow(app.GetWindow(edWinId));
#else
	std::vector<IRenderLayerRef*> gameCompositionLayerRefs = {
		IRenderFactory::CreateLayerRef<WorldRenderLayer, WorldRenderLayerRef>(render)
			->SetViewportSize({settings->PixelPerfectViewportWidth, settings->PixelPerfectViewportHeight})
			->SetCompensateAspectRatio(settings->CompensateAspectRatio)
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
	app.GetEngine()->SetWorld(app.GetEngine()->GetObjectLibrary()->CreateObject<World>("Transient World"));
	app.GetEngine()->LoadWorld(0xABCD0123);

	app.GetEngine()->GetWorld()->SetBackgroundColor(Vec3(0.75, 0.5, 0));
	app.GetEngine()->GetWorld()->Spawn<EntityTest>("TestEntity");
	EntitySpawnInfo spriteinfo {.Location = {0, 0, -10}};
	app.GetEngine()->GetWorld()->Spawn<SpriteEntity>("Sprite", spriteinfo);
	EntitySpawnInfo sprite2info {.Location = {8, 16, -5}};
	app.GetEngine()->GetWorld()->Spawn<SpriteEntity>("Sprite 2", sprite2info);
	EntitySpawnInfo sprite3info {.Location = {16, 8, -15}};
	app.GetEngine()->GetWorld()->Spawn<SpriteEntity>("Sprite 3", sprite3info);
	app.GetEngine()->GetWorld()->Spawn<EntTilemap>("Tilemap");
	CameraEntity* camera = app.GetEngine()->GetWorld()->Spawn<CameraEntity>("Camera");
	camera->SetCameraActive();

	app.GetEngine()->GetCanvasWidget()->AddChild(std::make_shared<PanelWidget>());

	std::unique_ptr<IDirectory> directory = IPlatform::Get()->OpenDirectory("./Content");
	std::unique_ptr<IDirectory> localDirectory = IPlatform::Get()->OpenLocalDirectory("./");
	std::unique_ptr<IDirectory> contentDirectory = IPlatform::Get()->OpenContentDirectory("./");

	Resource* testResourceShader = GEngine->GetAssetLoader()->LoadResource("Shaders/Test/Test.vert.spv");

	// Testing Sprite System
	ObjectRef<ObjTexture> texture = app.GetEngine()->GetObjectLibrary()->CreateObject<ObjTexture>("Test Texture");
	texture->SetPath("Textures/test.png");
	texture->LoadTexture();

	ObjectRef<ObjAtlas> atlas = app.GetEngine()->GetObjectLibrary()->CreateObject<ObjAtlas>("Test Atlas");
	atlas->SetTexture(texture);
	atlas->SetSpriteRect(0, {0, 0, 540/2, 540/2});
	atlas->SetSpriteRect(1, {540/2, 540/2, 540, 540});

	/*AssetFile assetFile("Editor/Content/AssetFile");
	assetFile.ReadFromDevice();*/

	//AssetLibrary::Get().LoadAsset("Editor/Content/Shaders/WidgetBrushDefault");

	app.ApplicationLoop();

	return 0;
}