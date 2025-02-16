#include "BetelImGui.h"
#include <Core/Window/WindowManager.h>

BetelImGuiEngine::BetelImGuiEngine()
{
}

BetelImGuiEngine::~BetelImGuiEngine()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void BetelImGuiEngine::ApplyTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_FrameBg]              = ImVec4(0.15, 0.15, 0.15, 1);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.075, 0.075, 0.075, 1);
    style.Colors[ImGuiCol_Button]               = ImVec4(0.1, 0.1, 0.1, 1);
    style.Colors[ImGuiCol_MenuBarBg]            = ImVec4(0.1, 0.1, 0.1, 1);
    style.Colors[ImGuiCol_Border]               = ImVec4(0.125, 0.125, 0.125, 1);
    style.Colors[ImGuiCol_Tab]                  = ImVec4(0.2, 0.2, 0.2, 1);
    style.Colors[ImGuiCol_TabHovered]           = ImVec4(0, 0.4, 0.8, 1);
    style.Colors[ImGuiCol_TabSelected]          = ImVec4(0, 0.4, 0.8, 1);
    style.Colors[ImGuiCol_TabActive]            = ImVec4(0, 0.647, 1, 1);
    style.Colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_TitleBg]              = ImVec4(0.096f, 0.096f, 0.096f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.10f, 0.10f, 0.10f, 0.51f);
    style.Colors[ImGuiCol_TitleBgActive]        = ImVec4(0.082f, 0.082f, 0.082f, 1.00f);
}

void BetelImGuiEngine::Initialize(SDL_Window* window, ImGui_ImplVulkan_InitInfo& initInfo, VkCommandBuffer singleTimeBuffer)
{
    // TODO: I18N integration
    // Handle fonts and locale detections
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable; 
    ImGui::GetIO().ConfigDockingTransparentPayload = true;
    ImGui::GetIO().IniFilename = "./Config/ImGui.ini";

    ApplyTheme();

    WindowManager::OnSDLEvent.BindRaw([](SDL_Event event){ImGui_ImplSDL2_ProcessEvent(&event);});

    ImGui_ImplSDL2_InitForVulkan(window);

    ImGui_ImplVulkan_Init(&initInfo);
    
    ImGui_ImplVulkan_CreateFontsTexture();
}