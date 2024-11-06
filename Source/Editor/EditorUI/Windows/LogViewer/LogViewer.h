#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>

class EditorLogViewer : public EditorToolkitWindow
{
    std::vector<const char*> messages;
public:
    EditorLogViewer() {
        Logger::OnLogMessage.BindMember(this, &EditorLogViewer::MessageCallback);
    }

    void MessageCallback(const char* type, const char* space, const char* text) {
        messages.push_back(text);
    }

    virtual void OnGUI(Window* window) {
        ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_Once);
        ImGui::SetNextWindowBgAlpha(1);
        if(ImGui::Begin("Log Viewer", 0, ImGuiWindowFlags_NoCollapse)){
            
            ImGui::SameLine();
            std::string buffer;
            buffer.resize(512);
            ImGui::InputText("##Filter", buffer.data(), 512);

            ImGui::SameLine();
            if(ImGui::Button("Filter")) {

            }

            ImGui::SameLine();
            if(ImGui::Button("Clear")) {
                messages.clear();
            }

            ImGui::BeginListBox("##LogBox", {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y});
            for(const char* message : messages) {
                // TODO: Text with selection
                ImGui::Text(message);
            }
            ImGui::EndListBox();
        }
        ImGui::End();
    }
};