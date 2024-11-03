#include "BetelImGui.h"

BetelImGuiEngine::BetelImGuiEngine()
{
}

BetelImGuiEngine::~BetelImGuiEngine()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void BetelImGuiEngine::Initialize(SDL_Window* window, ImGui_ImplVulkan_InitInfo& initInfo, VkCommandBuffer singleTimeBuffer)
{
    ImGui::CreateContext();
    
    ImGui_ImplSDL2_InitForVulkan(window);

    ImGui_ImplVulkan_Init(&initInfo);
    
    ImGui_ImplVulkan_CreateFontsTexture();
}