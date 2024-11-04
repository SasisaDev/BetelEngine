#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>

class EditorViewport : public EditorToolkitWindow
{
public:

    virtual void DrawGUI(Window* window) override { 
        if(Visible){
            this->OnGUI(window);
        } else {
            IRenderComposition* comp = GApplication->GetRender()->GetComposition(window->GetCompositionID());
            comp->GameViewport = {0};
        }
    }

    virtual void OnGUI(Window* window){
        IRenderComposition* comp = GApplication->GetRender()->GetComposition(window->GetCompositionID());

        ImGui::SetNextWindowBgAlpha(.0f);
        ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Once);
        if(ImGui::Begin("Game", 0, ImGuiWindowFlags_NoCollapse)){
            ImVec2 Pos = ImGui::GetWindowPos();
            ImVec2 Size = ImGui::GetWindowSize();
            comp->GameViewport = {{(int)Pos.x, (int)Pos.y}, {(unsigned int)Size.x, (unsigned int)Size.y}};
        }
        ImGui::End();
    }
};