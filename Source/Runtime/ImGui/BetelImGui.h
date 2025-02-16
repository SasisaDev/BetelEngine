#pragma once

#define VK_NO_PROTOTYPES
#define IMGUI_IMPL_VULKAN_USE_RENDERV

#include "imgui/imgui.h"
#include "imgui/imconfig.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_vulkan.h"

class BetelImGuiEngine {
    void ApplyTheme();
public:
    BetelImGuiEngine();
    ~BetelImGuiEngine();

    void Initialize(SDL_Window* window, ImGui_ImplVulkan_InitInfo& initInfo, VkCommandBuffer singleTimeBuffer = VK_NULL_HANDLE /*UNUSED*/);
};