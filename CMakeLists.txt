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

# Modules Sources
set(SrcGameFramework "Source/Runtime/GameFramework/Entities/Test/TestEntity.cpp" "Source/Runtime/GameFramework/Entities/Sprite/SpriteEntity.cpp") 

set(SrcAssets "Source/Runtime/Assets/AssetLibrary.cpp" "Source/Runtime/Assets/Asset.cpp") 

set(SrcRenderV "Source/Runtime/RenderV/Vulkan/vkloader.cpp" "Source/Runtime/RenderV/Render.cpp" "Source/Runtime/RenderV/Engine.cpp" 
    "Source/Runtime/RenderV/Composition.cpp" "Source/Runtime/RenderV/Layer.cpp" "Source/Runtime/RenderV/Utility.cpp" "Source/Runtime/RenderV/Objects/Shader.cpp"
    "Source/Runtime/RenderV/Objects/Buffers/Buffer.cpp" "Source/Runtime/RenderV/Objects/Material.cpp")

set(SrcEngine "Source/Runtime/Engine/Engine.cpp" "Source/Runtime/Engine/EngineDelegates.cpp" "Source/Runtime/Engine/Tickable.cpp") 

set(SrcWorld "Source/Runtime/World/RenderLayer/WorldRenderLayer.cpp" "Source/Runtime/World/World.cpp" "Source/Runtime/World/Entity.cpp") 

set(SrcUI "Source/Runtime/UI/RenderLayer/WidgetRenderLayer.cpp" "Source/Runtime/UI/Widget.cpp")

set(SrcPlatform "Source/Runtime/Platform/Interface/Files/IFile.cpp" "Source/Runtime/Platform/Interface/Files/IDirectory.cpp" "Source/Runtime/Platform/Platform.cpp"
                "Source/Runtime/Platform/Interface/Files/IPath.cpp" "Source/Runtime/Platform/Interface/IPlatform.cpp")

set(Sources ${SrcRenderV} ${SrcEngine} ${SrcWorld} ${SrcUI} ${SrcGameFramework} ${SrcAssets} ${SrcPlatform}
    "Source/Runtime/Core/main.cpp" "Source/Runtime/Core/Application/Application.cpp" 
    "Source/Runtime/Core/GuardedMain.cpp" "Source/Runtime/Log/Logger.cpp" "Source/Runtime/Core/Application/Arguments.cpp" 
    "Source/Runtime/Core/Window/Window.cpp" "Source/Runtime/Core/Window/WindowManager.cpp")

if(WIN32)
    set(Sources WIN32 ${Sources} "Source/Runtime/Platform/Windows/WindowsPlatform.cpp" "Source/Runtime/Platform/Windows/Files/WindowsFile.cpp")
endif()

# Game

add_executable(Game ${Sources})

target_link_libraries(Game Vulkan-Headers SDL2 glm-header-only)

target_include_directories(Game PRIVATE "Source/Runtime")

set_property(TARGET Game PROPERTY CXX_STANDARD 20)

# Editor
set(AssetFactoriesSrc "Source/Editor/AssetFactory/Default/ShaderFactory.cpp" )

set(EditorSources ${AssetFactoriesSrc} "Source/Editor/AssetFactory/AssetFactoryLibrary.cpp" ) 

add_executable(Editor ${Sources} ${EditorSources})

target_link_libraries(Editor Vulkan-Headers SDL2 glm-header-only)

target_include_directories(Editor PRIVATE "Source/Runtime" "Source/Editor")

target_compile_definitions(Editor PUBLIC EDITOR)

set_property(TARGET Editor PROPERTY CXX_STANDARD 20)

# Shaders
add_shaders(Shaders "Shaders/TriangleUpscale/TriangleUpscale.vert" "Shaders/TriangleUpscale/TriangleUpscale.frag" "Shaders/Test/Test.vert" "Shaders/Test/Test.frag")

# Assets
cook_assets(Cook "Content/" "Content/")