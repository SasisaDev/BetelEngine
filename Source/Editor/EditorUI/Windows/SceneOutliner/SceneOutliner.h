#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>

class EditorSceneOutliner : public EditorToolkitWindow
{
protected:
    virtual void EntityContainer(Entity* entity) {
        ImGui::Checkbox("##Visible", &entity->Visible);

        ImGui::SameLine();
        ImGui::Button(entity->GetDisplayName().c_str());
    }
public:

    virtual void OnGUI(Window* window){
        World* world = GApplication->GetEngine()->GetWorld();
        if(world == nullptr) {
            return;
        }

        ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_Once);
        ImGui::SetNextWindowBgAlpha(1);
        if(ImGui::Begin("World Outliner", 0, ImGuiWindowFlags_NoCollapse)){
            ImGui::Text("%s", world->GetWorldName().c_str());
            
            for(Entity* entity : world->GetEntities()) {
                EntityContainer(entity);
            }
        }
        ImGui::End();
    }
};