#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>
#include <Settings/SettingsManager.h>
#include <GameFramework/Settings/GameSettings.h>

#include <imgui/imgui_internal.h>

#include <ImGui/Betel/BetelInputs.h>

#include <optional>

class EditorGameSettings : public EditorToolkitWindow
{
    struct SettingsNode {
        Settings* settings;
        bool selected = false;
    };

    Text TabName = Text("EditorUI.GameSettings.TabName");

    const char* SettingsListName = "##SettingsList";
    const char* SettingsDataName = "##SettingsData";

    std::vector<SettingsNode> settings;
    SettingsNode* selectedSetting = nullptr;
public:
    EditorGameSettings() {
        Visible = false;

        settings.push_back({GApplication->GetSettings()->GetOrDefault<GameSettings>()});
    }

    void DrawSettingsListElement(SettingsNode &node)
    {

        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        if (node.selected)
            nodeFlags |= ImGuiTreeNodeFlags_Selected;

        ImGui::TreeNodeEx(node.settings->GetName().c_str(), nodeFlags, node.settings->GetName().c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            selectedSetting = &node;
            for(SettingsNode& otherNode : settings) {
                otherNode.selected = false;
            }
            node.selected = true;
        }
    }

    void DrawSettingsDomainField(std::string &name, std::string &value)
    { 
        BImGui::InputString(name.c_str(), value);
    }

    void DrawSettingsDomain(std::string &name, INIMap &values)
    {

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
            for(SettingsNode& setting : settings) {
                DrawSettingsListElement(setting);
            }   
        }
        ImGui::End();

        ImGui::SetNextWindowClass(&noTab_class);
        if(ImGui::Begin(SettingsDataName, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove)){
            if(selectedSetting != nullptr) {
                static std::string TestName = "Test";
                static std::string Test = "Lorem ipsum dolar sit amet";
                DrawSettingsDomainField(TestName, Test);
            }
        }
        ImGui::End();
    }
};