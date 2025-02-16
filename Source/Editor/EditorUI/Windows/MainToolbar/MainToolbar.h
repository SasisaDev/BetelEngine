#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>
#include <Debug/DebugImageLoader.h>
#include <ImGui/Betel/BetelImageButton.h>
#include <ImGui/Betel/BetelImages.h>

class EditorMainToolbar : public EditorToolkitWindow
{
    ImTextureID SaveImage;
public:
    const char* GetName()override{return "Action Toolbar";}

    EditorMainToolbar()
    {
        
    }

    virtual void OnGUI(Window* window) {
        static bool first_init = true;
        if(first_init) {
           SaveImage = BImGui::GetEdImage(BImGui::Img::Save32Icon);

            first_init = false;
        }

        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResize;
        ImGui::SetNextWindowClass(&window_class);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.08f, 0.08f, 1));
        if(ImGui::Begin(GetName(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize)){    
            ImGui::ImageButton("##SaveLevel", (ImTextureID)SaveImage, ImVec2(32, 32));
        }
        ImGui::End();
        ImGui::PopStyleColor();
    }
};