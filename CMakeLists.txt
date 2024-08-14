cmake_minimum_required(VERSION 3.21)

project ("Arteria")

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
set(SrcRenderV "Source/Runtime/RenderV/Vulkan/vkloader.cpp" "Source/Runtime/RenderV/Render.cpp" "Source/Runtime/RenderV/Engine.cpp" 
    "Source/Runtime/RenderV/Composition.cpp" "Source/Runtime/RenderV/Layer.cpp" "Source/Runtime/RenderV/Utility.cpp" "Source/Runtime/RenderV/Objects/Shader.cpp")

set(SrcEngine "Source/Runtime/Engine/Engine.cpp" "Source/Runtime/Engine/EngineDelegates.cpp" "Source/Runtime/Engine/Tickable.cpp") 

set(SrcWorld "Source/Runtime/World/RenderLayer/WorldRenderLayer.cpp") 

set(Sources ${SrcRenderV} ${SrcEngine} ${SrcWorld} "Source/Runtime/Core/main.cpp" "Source/Runtime/Core/Application/Application.cpp" 
    "Source/Runtime/Core/GuardedMain.cpp" "Source/Runtime/Platform/Platform.cpp" "Source/Runtime/Log/Logger.cpp" 
    "Source/Runtime/Delegate/Delegate.cpp" "Source/Runtime/Core/Window/Window.cpp")

if(WIN32)
    set(Sources WIN32 ${Sources} "Source/Runtime/Platform/Windows/WindowsPlatform.cpp")
endif()

set(EditorSources "") 

# Game

add_executable(Game ${Sources})

target_link_libraries(Game Vulkan-Headers SDL2 glm-header-only)

target_include_directories(Game PRIVATE "Source/Runtime")

set_property(TARGET Game PROPERTY CXX_STANDARD 20)

# Editor

add_executable(Editor ${Sources} ${EditorSources})

target_link_libraries(Editor Vulkan-Headers SDL2 glm-header-only)

target_include_directories(Editor PRIVATE "Source/Runtime" "Source/Editor")

target_compile_definitions(Editor PUBLIC EDITOR)

set_property(TARGET Editor PROPERTY CXX_STANDARD 20)