#pragma once

#include <Toolkit/ToolkitWindow.h>

#include <ImGui/Betel/BetelColors.h>

class ObjectEditView : public EditorToolkitWindow
{
protected:
    std::string Title;
    bool bResizable = false;
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
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;

        if(!bResizable) {
            ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);
            flags |= ImGuiWindowFlags_NoResize;
        }

        if (ImGui::Begin(Title.c_str(), &Visible, flags))
        {
            OnEditViewGUI(window);

            // Draw Buttons
            float buttonWidth1 = ImGui::CalcTextSize("Cancel").x + style.FramePadding.x * 2.f;
            float buttonWidth2 = ImGui::CalcTextSize("Save").x + style.FramePadding.x * 2.f;
            float widthNeeded = buttonWidth1 + style.ItemSpacing.x + buttonWidth2 + GetCustomControlButtonsWidth(style);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);

            DrawCustomControlButtons();

            if(ImGui::Button("Cancel")) {
                Visible = false;
            }
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, BImGui::Colors::ContrastColor);
            if(ImGui::Button("Save")) {
                if(SaveObject()){
                    Visible = false;
                } else {
                    OnSaveError();
                }
            }
            ImGui::PopStyleColor();
        }
        ImGui::End();
    }

    // Returns true when success, otherwise returns false
    virtual bool SaveObject()
    {
        return true;
    }

    virtual void OnSaveError() {}
};