#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>

class EditorLog : public EditorToolkitWindow
{
    
public:
    EditorLog() {
        Logger::OnLogMessage.BindMember(this, &EditorLog::MessageCallback);
    }

    void MessageCallback(const char* type, const char* space, const char* text) {

    }

    virtual void OnGUI(Window* window) {
        ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_Once);
        ImGui::SetNextWindowBgAlpha(1);
        if(ImGui::Begin("Log Viewer", 0, ImGuiWindowFlags_NoCollapse)){
            
        }
        ImGui::End();
    }
};