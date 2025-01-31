#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>
#include <EditorUI/EditorImageLoader.h>
#include <EditorUI/WindowLibrary/BetelImageButton.h>

class EditorMainToolbar : public EditorToolkitWindow
{
    EditorTextureData SaveImage;
public:
    const char* GetName()override{return "Action Toolbar";}

    EditorMainToolbar()
    {
        
    }

    virtual void OnGUI(Window* window) {
        static bool first_init = true;
        if(first_init) {
            EditorImageLoader::Get().LoadTextureFromFile("./Editor/Save32.png", &SaveImage);

            first_init = false;
        }

        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResize;
        ImGui::SetNextWindowClass(&window_class);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.08f, 0.08f, 1));
        if(ImGui::Begin(GetName(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize)){    
            ImGui::ImageButton("##SaveLevel", (ImTextureID)SaveImage.DS, ImVec2(SaveImage.Width, SaveImage.Height));
        }
        ImGui::End();
        ImGui::PopStyleColor();
    }
};