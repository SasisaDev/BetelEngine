#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>

#include <imgui/imgui_internal.h>

class EditorGameSettings : public EditorToolkitWindow
{
    Text TabName = Text("EditorUI", "GameSettings", "TabName");

    const char* SettingsListName = "##SettingsList";
    const char* SettingsDataName = "##SettingsData";
public:
    EditorGameSettings() {
        Visible = false;
    }

    virtual void OnGUI(Window* window) {
        ImGui::SetNextWindowSize(ImVec2(720, 340), ImGuiCond_Once);
        ImGui::SetNextWindowBgAlpha(1);
        if(ImGui::Begin(TabName.Get().c_str(), &Visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking)){
            ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
            ImGuiViewport *viewport = ImGui::GetWindowViewport();

            ImGuiID dockspace_id = ImGui::GetID("GameSettingsDockSpace");
            ImGui::SetNextWindowBgAlpha(.0f);
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags, nullptr);

            static auto first_time = true;
            if (first_time)
            {
                first_time = false;

                ImGui::DockBuilderRemoveNode(dockspace_id);
                ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

                auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);

                // we now dock our windows into the docking node we made above
                ImGui::DockBuilderDockWindow(SettingsDataName, dockspace_id);
                ImGui::DockBuilderDockWindow(SettingsListName, dock_id_left);
                ImGui::DockBuilderFinish(dockspace_id);
            }
        }
        ImGui::End();

        ImGuiWindowClass noTab_class;
        noTab_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
        
        ImGui::SetNextWindowClass(&noTab_class);
        if(ImGui::Begin(SettingsListName, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove)){
            
        }
        ImGui::End();

        ImGui::SetNextWindowClass(&noTab_class);
        if(ImGui::Begin(SettingsDataName, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove)){
            
        }
        ImGui::End();
    }
};