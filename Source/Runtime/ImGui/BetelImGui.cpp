#include "BetelImGui.h"
#include <Core/Window/WindowManager.h>
#include <cassert>

#include <imgui/imgui_internal.h>
#include <imgui/imconfig.h>

#include <Engine/Engine.h>
#include <World/World.h>

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

void *BetelImGuiEngine::WorldViewSettingsHandler_ReadOpen(ImGuiContext *, ImGuiSettingsHandler *, const char *name)
{
    ImGuiID id = ImHashStr(name);

    return nullptr;
}

void BetelImGuiEngine::WorldViewSettingsHandler_ReadLine(ImGuiContext *, ImGuiSettingsHandler *, void *entry, const char *line)
{

}

void BetelImGuiEngine::WorldViewSettingsHandler_WriteAll(ImGuiContext *ctx, ImGuiSettingsHandler *handler, ImGuiTextBuffer *buf)
{
    buf->appendf("[%s] [%s]\n", handler->TypeName, "Placeholder");
    // FIXME
    /*assert(GEngine != nullptr);
    
    for(Object* object : GEngine->GetObjectLibrary()->GetObjectsOfTypeID(World::GetStaticType()))
    {
        if(World* world = dynamic_cast<World*>(object))
        {
            buf->appendf("[%s] [%s]\n", handler->TypeName, world->GetName().c_str());
            buf->appendf("EditorCameraPosition=%i,%i\n", static_cast<int>(world->GetSceneView().ViewOrigin.x), static_cast<int>(world->GetSceneView().ViewOrigin.y));
        }
    }*/
}

void BetelImGuiEngine::Initialize(SDL_Window* window, ImGui_ImplVulkan_InitInfo& initInfo, VkCommandBuffer singleTimeBuffer)
{
    // TODO: I18N integration
    // Handle fonts and locale detections
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable; 
    ImGui::GetIO().ConfigDockingTransparentPayload = true;
    ImGui::GetIO().IniFilename = "./Config/ImGui.ini";
#   ifndef EDITOR
    ImGui::GetIO().IniFilename = "";
#   endif

    ApplyTheme();

    WindowManager::OnSDLEvent.BindRaw([](SDL_Event event){ImGui_ImplSDL2_ProcessEvent(&event);});

    ImGui_ImplSDL2_InitForVulkan(window);

    ImGui_ImplVulkan_Init(&initInfo);
    
    ImGui_ImplVulkan_CreateFontsTexture();

    ImGuiSettingsHandler world_view_ini_handler;
    world_view_ini_handler.TypeName = "WorldView";
    world_view_ini_handler.TypeHash = ImHashStr("WorldView");
    world_view_ini_handler.ReadOpenFn = WorldViewSettingsHandler_ReadOpen;
    world_view_ini_handler.ReadLineFn = WorldViewSettingsHandler_ReadLine;
    world_view_ini_handler.WriteAllFn = WorldViewSettingsHandler_WriteAll;
    ImGui::AddSettingsHandler(&world_view_ini_handler);
}