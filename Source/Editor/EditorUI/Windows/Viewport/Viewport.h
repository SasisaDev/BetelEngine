#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>
#include <Editor/Editor.h>
#include "../../EditorImageLoader.h"

class EditorViewport : public EditorToolkitWindow
{
    EditorTextureData ShowOverlayImageW{};
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
        static bool first_init = true;
        if(first_init) {
            EditorImageLoader::Get().LoadTextureFromFile("./Editor/ShowOverlayW16.png", 16, 16, &ShowOverlayImageW);

            first_init = false;
        }

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
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15, 0.15, 0.15, 0.4));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15, 0.15, 0.15, 0.5));
            ImGui::SetNextItemWidth(175);
            std::vector<const char*> modeNames;
            for(EditorMode* mode : Editor::Get()->Modes) {
                modeNames.push_back(mode->GetName().Get().c_str());
            }
            ImGui::Combo("##EditorMode", &Editor::Get()->CurrentActiveMode, modeNames.data(), IM_ARRAYSIZE(modeNames.data()));
            ImGui::PopStyleColor(2);

            // Show Editor Overlay Button
            ImGui::SameLine();
            if(Editor::Get()->bShowOverlay){
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.647, 1, 0.9));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.6));
            }
            if(ImGui::ImageButton("##ShowOverlay", (ImTextureID)ShowOverlayImageW.DS, ImVec2(14,14))) {
                Editor::Get()->bShowOverlay = !Editor::Get()->bShowOverlay;
            }
            ImGui::PopStyleColor(1);

        }
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(1);
        ImGui::End();
    }
};