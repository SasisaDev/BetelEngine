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
            comp->SetGameViewport({0});
        }
    }

    virtual void OnGUI(Window* window){
        IRenderComposition* comp = GApplication->GetRender()->GetComposition(window->GetCompositionID());

        // Fix docked window background being different from the undocked
        static ImGuiWindowClass vpClass;
        vpClass.ClassId = ImGui::GetID("GameViewport");
        vpClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoUndocking;

        ImGui::SetNextWindowClass(&vpClass);
        //ImGui::SetNextWindowBgAlpha(1.f);
        ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Once);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
        if(ImGui::Begin("Game", 0, ImGuiWindowFlags_NoCollapse)){
            if(ImGui::IsWindowDocked()) {
                ImGuiID dockID = ImGui::GetWindowDockID();
                
            }

            ImRect workRect = ImGui::GetCurrentWindow()->WorkRect;
            comp->SetGameViewport({{(int)workRect.GetTL().x, (int)workRect.GetTL().y}, {(unsigned int)workRect.GetWidth(), (unsigned int)workRect.GetHeight()}});
        }
        ImGui::PopStyleColor(1);
        ImGui::PopStyleVar(1);
        ImGui::End();
    }
};