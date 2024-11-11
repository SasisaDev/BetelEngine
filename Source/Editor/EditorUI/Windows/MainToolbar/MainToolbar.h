#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>

class EditorMainToolbar : public EditorToolkitWindow
{
    
public:

    virtual void OnGUI(Window* window) {
        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
        ImGui::SetNextWindowClass(&window_class);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.08f, 0.08f, 1));
        if(ImGui::Begin("Action Toolbar", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration)){    
            
        }
        ImGui::End();
        ImGui::PopStyleColor();
    }
};