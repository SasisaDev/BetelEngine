#pragma once

#include <Toolkit/Toolkit.h>
#include <imgui.h>
#include <Log/Logger.h>

class EditorToolkitBase : public EditorToolkit {
public:

    virtual void OnGUI() override {
        ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_Once);
        if(ImGui::Begin("Test Window")){
            ImGui::Text("Some random text");
            if(ImGui::Button("Random Function")) {
                LOG(Log, LogTest, "ImGui button pressed");
            }
        }
        ImGui::End();
    }
};