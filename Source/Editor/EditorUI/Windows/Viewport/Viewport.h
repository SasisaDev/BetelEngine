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

        // Constant values
        static const int32_t WindowPadding = 5;

        // Fix docked window background being different from the undocked
        static ImGuiWindowClass vpClass;
        vpClass.ClassId = ImGui::GetID("GameViewport");
        vpClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoUndocking;

        ImGui::SetNextWindowClass(&vpClass);
        //ImGui::SetNextWindowBgAlpha(1.f);
        ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Once);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(WindowPadding, WindowPadding));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
        if(ImGui::Begin(GetName(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)){
            if(ImGui::IsWindowDocked()) {
                ImGuiID dockID = ImGui::GetWindowDockID();
                
            }

            ImRect workRect = ImGui::GetCurrentWindow()->WorkRect;
            comp->SetGameViewport({{(int)workRect.GetTL().x - WindowPadding, (int)workRect.GetTL().y - WindowPadding}, {(unsigned int)workRect.GetWidth() + WindowPadding * 2, (unsigned int)workRect.GetHeight() + WindowPadding * 2}});

            // In editor mod set, if the game is focused or not
            if(GApplication && GApplication->GetEngine())
                GApplication->GetEngine()->SetGameFocused(ImGui::IsWindowFocused());

            // Draw Toolbar Elements
            // TODO: Editor Mod Fetch & Registration
            ImGui::SetNextItemWidth(175);
            static const char* comboItems[]{"Object Mode","Tilemap Mode","NPC Mode"};
            static int selectedComboItem = 0;
            ImGui::Combo("##EditorMode", &selectedComboItem, comboItems, IM_ARRAYSIZE(comboItems));
            
        }
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(1);
        ImGui::End();
    }
};