#pragma once

#include <sstream>
#include <iomanip>

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>

#include <Engine/Engine.h>

class EditorTranslator : public EditorToolkitWindow
{
    std::string utf8Japanese;
public:
    EditorTranslator() {
        Visible = false;
        utf8Japanese = GEngine->GetTextManager().GetLocales()[1]->Get("LocaleName");
    }

    virtual void OnGUI(Window* window) {
        ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_Once);
        ImGui::SetNextWindowBgAlpha(1);
        if(ImGui::Begin("Translation example : 例えば、日本語", &Visible)){
            ImGui::InputText("test", utf8Japanese.data(), 10);
        }
        ImGui::End();
    }
};