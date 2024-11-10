#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>

#include <i18n/Text.h>

class EditorSceneOutliner : public EditorToolkitWindow
{
    Text TabName = Text("EditorUI", "SceneOutliner", "TabName");
protected:
    virtual void EntityContainer(Entity* entity, bool indent = false) {
        if(indent) 
            ImGui::Indent();

        if(entity->GetChildren().size() > 0) {
            if(ImGui::Button(">")) {

            }
        } else {
            ImGui::Button(" ");
        }
        ImGui::SameLine();
        ImGui::Checkbox("##Visible", &entity->Visible);

        ImGui::SameLine();
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0, 0.5});
        ImGui::Button(entity->GetDisplayName().c_str(), {ImGui::GetContentRegionAvail().x, 0});
        ImGui::PopStyleVar();
        
        if(indent) 
            ImGui::Unindent();
    }
public:

    virtual void OnGUI(Window* window){
        World* world = GApplication->GetEngine()->GetWorld();
        if(world == nullptr) {
            return;
        }

        ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_Once);
        ImGui::SetNextWindowBgAlpha(1);
        if(ImGui::Begin(TabName.Get().c_str(), 0, ImGuiWindowFlags_NoCollapse)){
            ImGui::Text("%s", world->GetWorldName().c_str());
            
            for(Entity* entity : world->GetEntities()) {
                EntityContainer(entity);
            }
        }
        ImGui::End();
    }
};