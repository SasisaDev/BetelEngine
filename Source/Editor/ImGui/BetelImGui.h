#pragma once

#define VK_NO_PROTOTYPES
#define IMGUI_IMPL_VULKAN_USE_RENDERV

#include "imgui.h"
#include "imconfig.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_vulkan.h"

class BetelImGuiEngine {

public:
    BetelImGuiEngine();
    ~BetelImGuiEngine();

    void Initialize(SDL_Window* window, ImGui_ImplVulkan_InitInfo& initInfo, VkCommandBuffer singleTimeBuffer = VK_NULL_HANDLE /*UNUSED*/);
};