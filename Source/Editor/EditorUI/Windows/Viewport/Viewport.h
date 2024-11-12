#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>

class EditorViewport : public EditorToolkitWindow
{
public:
    const char* GetName()override{return "Game";}

    virtual void DrawGUI(Window* window) override { 
        if(Visible){
            this->OnGUI(window);
        } else {
            IRenderComposition* comp = GApplication->GetRender()->GetComposition(window->GetCompositionID());
            comp->SetGameViewport({0});
            if(GApplication && GApplication->GetEngine())
                GApplication->GetEngine()->SetGameFocused(false);
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
        if(ImGui::Begin(GetName(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)){
            if(ImGui::IsWindowDocked()) {
                ImGuiID dockID = ImGui::GetWindowDockID();
                
            }

            ImRect workRect = ImGui::GetCurrentWindow()->WorkRect;
            comp->SetGameViewport({{(int)workRect.GetTL().x, (int)workRect.GetTL().y}, {(unsigned int)workRect.GetWidth(), (unsigned int)workRect.GetHeight()}});

            // In editor mod set, if the game is focused or not
            if(GApplication && GApplication->GetEngine())
                GApplication->GetEngine()->SetGameFocused(ImGui::IsWindowFocused());

            // Draw Toolbar Elements
            
        }
        ImGui::PopStyleColor(1);
        ImGui::PopStyleVar(1);
        ImGui::End();
    }
};