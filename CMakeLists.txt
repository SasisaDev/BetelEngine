cmake_minimum_required(VERSION 3.21)

project ("Betel")

set(BUILD_CONFIG "$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>")

include("CMake/VulkanCompileShaders.cmake")
include("CMake/CookAssets.cmake")
include(FetchContent)

FetchContent_Declare(
  vulkan
  GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
  GIT_TAG        5677bafb820e476441e9e1f745371b72133407d3 # v1.3.285
)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        1.0.1 # 1.0.1
)

FetchContent_Declare(
  lua
  GIT_REPOSITORY https://github.com/lua/lua.git
  GIT_TAG        v5.4.6  # v5.4.6 
)

FetchContent_Declare(
  sdl
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git 
  GIT_TAG        release-2.30.3 # release-2.30.3
)

FetchContent_MakeAvailable(vulkan sdl glm lua)

# Disable reserved literals error
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reserved-user-defined-literal")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-reserved-user-defined-literal")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-literal-suffix -Wliteral-suffix")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-literal-suffix -Wliteral-suffix")
endif()

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# Third Party Sources
set(SrcStbImage "Source/ThirdParty/stb/stb_image.cpp")

set(ImguiSrc "Source/ThirdParty/imgui/imgui_demo.cpp" "Source/ThirdParty/imgui/imgui_tables.cpp" 
             "Source/ThirdParty/imgui/imgui_draw.cpp" "Source/ThirdParty/imgui/imgui_widgets.cpp" 
             "Source/ThirdParty/imgui/imgui.cpp" "Source/ThirdParty/imgui/backends/imgui_impl_sdl2.cpp"
             "Source/ThirdParty/imgui/backends/imgui_impl_vulkan.cpp")
set(ImguiBetelSrc "Source/Runtime/ImGui/RenderLayer/ImGuiLayer.cpp" "Source/Runtime/ImGui/BetelImGui.cpp" "Source/Runtime/ImGui/Betel/BetelImages.cpp"
                  "Source/Runtime/ImGui/Betel/BetelDeferred.cpp" "Source/Runtime/ImGui/Betel/BetelInputs.cpp"
                  "Source/Runtime/ImGui/Betel/BetelDrag.cpp")

# LZ4 Source
set(SrcLZ4 "Source/ThirdParty/lz4/lz4.c" "Source/ThirdParty/lz4/lz4file.c" "Source/ThirdParty/lz4/xxhash.c" "Source/ThirdParty/lz4/lz4hc.c" "Source/ThirdParty/lz4/lz4frame.c")

# Modules Sources
set(SrcGameFramework "Source/Runtime/GameFramework/Entities/Test/TestEntity.cpp" "Source/Runtime/GameFramework/Entities/Sprite/SpriteEntity.cpp"
     "Source/Runtime/GameFramework/Assets/Sprite/Sprite.cpp"
    "Source/Runtime/Materials/Assets/Material.cpp" "Source/Runtime/Materials/Assets/Shader.cpp"
    "Source/Runtime/GameFramework/Assets/Atlas/Atlas.cpp" "Source/Runtime/GameFramework/Assets/Texture/Texture.cpp"
    "Source/Runtime/GameFramework/Assets/Animation/Animation.cpp"
    "Source/Runtime/GameFramework/Entities/Character/Character.cpp" "Source/Runtime/GameFramework/Entities/Controller/Controller.cpp"

    "Source/Runtime/GameFramework/Entities/Tilemap/Tilemap.cpp" "Source/Runtime/GameFramework/Entities/Tilemap/TilemapLayerAtlas.cpp") 

set(SrcAssets "Source/Runtime/Assets/AssetLibrary.cpp" "Source/Runtime/Assets/Asset.cpp" "Source/Runtime/Assets/AssetFile/AssetFile.cpp")

set(SrcAssetLoader "Source/Runtime/AssetLoader/AssetLoader.cpp" )

set(SrcRenderV "Source/Runtime/RenderV/Vulkan/vkloader.cpp" "Source/Runtime/RenderV/Render.cpp" "Source/Runtime/RenderV/Engine.cpp" 
    "Source/Runtime/RenderV/Composition.cpp" "Source/Runtime/RenderV/Layer.cpp" "Source/Runtime/RenderV/Utility.cpp" "Source/Runtime/RenderV/Objects/Shader.cpp"
    "Source/Runtime/RenderV/Objects/Buffers/Buffer.cpp" "Source/Runtime/RenderV/Objects/Material.cpp" "Source/Runtime/RenderV/Objects/Images/SamplerTexture.cpp")

set(SrcEngine "Source/Runtime/Engine/Engine.cpp" "Source/Runtime/Engine/EngineDelegates.cpp" "Source/Runtime/Engine/Tickable.cpp")

set(SrcObject "Source/Runtime/Object/Object.cpp" "Source/Runtime/Object/ObjectRef.cpp" "Source/Runtime/Object/ObjectLibrary.cpp") 

set(SrcAudio "Source/Runtime/Audio/Audio.cpp") 

set(SrcWorld "Source/Runtime/World/RenderLayer/WorldRenderLayer.cpp" "Source/Runtime/World/World.cpp" "Source/Runtime/World/Entity.cpp" 
             "Source/Runtime/World/Entities/Camera/Camera.cpp") 

set(SrcUI "Source/Runtime/UI/RenderLayer/WidgetRenderLayer.cpp" "Source/Runtime/UI/Widget.cpp" "Source/Runtime/UI/Widgets/Panel/Panel.cpp")

set(SrcInput "Source/Runtime/Input/InputManager.cpp")

set(SrcI18N "Source/Runtime/i18n/Text.cpp")

set(SrcTimer "Source/Runtime/Timer/Timer.cpp")

set(SrcPlatform "Source/Runtime/Platform/Interface/Files/IFile.cpp" "Source/Runtime/Platform/Interface/Files/IDirectory.cpp" "Source/Runtime/Platform/Platform.cpp"
                "Source/Runtime/Platform/Interface/Files/IPath.cpp" "Source/Runtime/Platform/Interface/IPlatform.cpp")

set(Sources ${SrcRenderV} ${SrcObject} ${SrcEngine} ${SrcWorld} ${SrcUI} ${SrcGameFramework} ${SrcAssets} ${SrcPlatform} ${SrcInput} ${SrcI18N} ${SrcLZ4}
    ${SrcAssetLoader} ${SrcStbImage} ${SrcTimer} ${SrcAudio}
    "Source/Runtime/Core/main.cpp" "Source/Runtime/Core/Application/Application.cpp" 
    "Source/Runtime/Core/GuardedMain.cpp" "Source/Runtime/Log/Logger.cpp" "Source/Runtime/Core/Application/Arguments.cpp" 
    "Source/Runtime/Core/Window/Window.cpp" "Source/Runtime/Core/Window/WindowManager.cpp")

# Add Imgui in debug build
set(Sources ${Sources} "$<$<CONFIG:Debug>:${ImguiSrc}>" "$<$<CONFIG:Debug>:${ImguiBetelSrc}>" "$<$<CONFIG:Debug>:Source/Runtime/Debug/DebugImGui/DebugImGui.cpp>")

if(WIN32)
    set(Sources WIN32 ${Sources} "Source/Runtime/Platform/Windows/WindowsPlatform.cpp" "Source/Runtime/Platform/Windows/Files/WindowsFile.cpp")
endif()
if(CMAKE_HOST_SYSTEM MATCHES Linux)
  set(Sources ${Sources} "Source/Runtime/Platform/Linux/LinuxPlatform.cpp")
endif()

# Game

add_executable(Game ${Sources})

target_link_libraries(Game Vulkan-Headers SDL2::SDL2-static glm-header-only)

target_include_directories(Game PRIVATE "Source/Runtime")
target_include_directories(Game PRIVATE "Source/ThirdParty")

set_property(TARGET Game PROPERTY CXX_STANDARD 20)

# Editor

set(AssetFactoriesSrc "Source/Editor/AssetFactory/Default/ShaderFactory.cpp" )

set(ArgumentEventsSrc "Source/Editor/ArgumentEvents/UseAssetFactoryEvent.cpp")

set(TransactionQueueSrc "Source/Editor/TransactionQueue/Context.cpp" "Source/Editor/TransactionQueue/Queue/Queue.cpp")

set(EditorSources ${AssetFactoriesSrc} ${ArgumentEventsSrc} ${ImguiSrc} ${ImguiBetelSrc} ${TransactionQueueSrc}
    "Source/Editor/ToolkitRenderLayer/ToolkitRenderLayer.cpp" "Source/Editor/AssetFactory/AssetFactoryLibrary.cpp" "Source/Editor/Editor/EditorDelegates.cpp"
    "Source/Editor/Editor/Editor.cpp" "Source/Editor/Editor/Modes/ObjectMode.cpp" "Source/Editor/EditorUI/ObjectEditViews/ObjectEditViewsFactory.cpp") 

add_executable(Editor ${Sources} ${EditorSources})

target_link_libraries(Editor Vulkan-Headers SDL2::SDL2-static glm-header-only)

target_include_directories(Editor PRIVATE "Source/Runtime" "Source/Editor")

target_include_directories(Editor PRIVATE "Source/ThirdParty")

target_compile_definitions(Editor PUBLIC EDITOR)

set_property(TARGET Editor PROPERTY CXX_STANDARD 20)

# Address Sanitizer Builds

add_executable(EditorASAN ${Sources} ${EditorSources})

target_link_libraries(EditorASAN Vulkan-Headers SDL2::SDL2-static glm-header-only)

target_include_directories(EditorASAN PRIVATE "Source/Runtime" "Source/Editor")

target_include_directories(EditorASAN PRIVATE "Source/ThirdParty")

target_compile_definitions(EditorASAN PUBLIC EDITOR)

set_property(TARGET EditorASAN PROPERTY CXX_STANDARD 20)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  message(STATUS "ASAN: Detected MSVC")
  target_compile_options(EditorASAN PUBLIC "/MTd" "/fsanitize=address" "/Zi" "/sdl" "/permissive-") 
  target_link_options(EditorASAN PUBLIC "/INCREMENTAL:NO")
  target_link_options(EditorASAN PUBLIC "/subsystem:console")
else()
  message(STATUS "ASAN: Detected GNU or LLVM")
  target_compile_options(EditorASAN PUBLIC "-fsanitize=address") 
  target_link_options(EditorASAN PUBLIC "-fsanitize=address")
endif()
#set(CMAKE_EXE_LINKER_FLAGS "/subsystem:console ${CMAKE_EXE_LINKER_FLAGS} /subsystem:console")

# Shaders
add_shaders(Shaders "Shaders/TriangleUpscale/TriangleUpscale.vert" "Shaders/TriangleUpscale/TriangleUpscale.frag" "Shaders/Test/Test.vert" "Shaders/Test/Test.frag"
                    "Shaders/UI_Brush_Texture/UI_Brush_Texture.frag" "Shaders/UI_Brush_Texture/UI_Brush_Texture.vert" "Shaders/SpriteMasked/SpriteMasked.frag" "Shaders/SpriteMasked/SpriteMasked.vert"
                    "Shaders/Tilemap_LayerAtlasInstanced/Tilemap_LayerAtlasInstanced.frag" "Shaders/Tilemap_LayerAtlasInstanced/Tilemap_LayerAtlasInstanced.vert")

# Assets
cook_assets(Cook "Content/" "Content/")