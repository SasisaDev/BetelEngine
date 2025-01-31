#pragma once

#include <Toolkit/ToolkitWindow.h>

#include <EditorUI/WindowLibrary/BetelColors.h>

class ObjectEditView : public EditorToolkitWindow
{
protected:
    std::string Title;
public:
    ObjectEditView(const char* title)
        : Title(title)
    {
    }

    ~ObjectEditView()
    {
    }

    virtual void OnEditViewGUI(Window* window)
    {

    }

    virtual float GetCustomControlButtonsWidth(const ImGuiStyle& style) { return 0; }

    virtual void DrawCustomControlButtons() {}

    virtual void OnGUI(Window *window) override
    {
        ImGuiStyle& style = ImGui::GetStyle();

        ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Once);
        if (ImGui::Begin(Title.c_str(), &Visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
        {
            OnEditViewGUI(window);

            // Draw Buttons
            static float buttonWidth1 = ImGui::CalcTextSize("Cancel").x + style.FramePadding.x * 2.f;
            static float buttonWidth2 = ImGui::CalcTextSize("Save").x + style.FramePadding.x * 2.f;
            static float widthNeeded = buttonWidth1 + style.ItemSpacing.x + buttonWidth2 + GetCustomControlButtonsWidth(style);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);

            DrawCustomControlButtons();

            if(ImGui::Button("Cancel")) {
                Visible = false;
            }
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, BImGui::Colors::ContrastColor);
            if(ImGui::Button("Save")) {
                SaveObject();
                Visible = false;
            }
            ImGui::PopStyleColor();
        }
        ImGui::End();
    }

    virtual void SaveObject()
    {
        
    }
};