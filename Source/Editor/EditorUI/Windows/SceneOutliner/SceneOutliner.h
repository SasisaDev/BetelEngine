#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>

class EditorSceneOutliner : public EditorToolkitWindow
{
public:

    virtual void OnGUI(){
        World* world = GApplication->GetEngine()->GetWorld();
        if(world == nullptr) {
            return;
        }

        ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_Once);
        if(ImGui::Begin("World Outliner", 0, ImGuiWindowFlags_NoCollapse)){
            ImGui::Text("%s", world->GetWorldName().c_str());
        }
        ImGui::End();
    }
};