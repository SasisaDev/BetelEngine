#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>

class EditorLogViewer : public EditorToolkitWindow
{
    std::vector<std::string> messages;
    bool AutoScroll = true; 
    bool ScrollNow = true; 
public:
    EditorLogViewer() {
        Logger::OnLogMessage.BindMember(this, &EditorLogViewer::MessageCallback);
    }

    virtual const char* GetName() override {return "Log Viewer";}

    void MessageCallback(const char* timestamp, const char* space, const char* text) {
        messages.push_back(std::string("[") + timestamp + "] [" + space + "] " + text);
        ScrollNow = true;
    }

    virtual void OnGUI(Window* window) override {
        ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_Once);
        ImGui::SetNextWindowBgAlpha(1);
        if(ImGui::Begin(GetName(), 0, ImGuiWindowFlags_NoCollapse)){
            
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

            if(ImGui::BeginListBox("##LogBox", {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y})) {
                for(std::string message : messages) {
                    // TODO: Text with selection
                    ImGui::Text("%s", message.c_str());
                }

                if(AutoScroll && ScrollNow && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                    ImGui::SetScrollHereY(1.0f);
                    ScrollNow = false;
                }
                ImGui::EndListBox();
            }
        }
        ImGui::End();
    }
};