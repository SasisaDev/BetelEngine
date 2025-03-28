#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>

#include <i18n/Text.h>

#include <World/World.h>

class EditorSceneOutliner : public EditorToolkitWindow
{
    Text TabName = Text("EditorUI.SceneOutliner.TabName");
    std::string TranslatedName;
protected:
    virtual void EntityContainer(Entity* entity, bool indent = false) {
        if(indent) 
            ImGui::Indent();

        if(entity->GetChildren().size() > 0) {
            if(ImGui::Button((">##" + entity->GetDisplayName()).c_str())) {

            }
        } else {
            ImGui::Button(" ");
        }
        ImGui::SameLine();
        ImGui::Checkbox(("##Visible" + entity->GetDisplayName()).c_str(), &entity->bVisible);

        ImGui::SameLine();
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0, 0.5});

        std::string displayName = entity->GetDisplayName().c_str();
        std::string hexID;
        hexID.resize(2+8);
        sprintf(hexID.data(), "0x%08X", entity->GetID());
        displayName += "(" + hexID + ")";
        ImGui::Button(displayName.c_str(), {ImGui::GetContentRegionAvail().x, 0});
        
        ImGui::PopStyleVar();
        
        if(indent) 
            ImGui::Unindent();
    }
public:
    EditorSceneOutliner() {
        TranslatedName = TabName.Get();
    }

    virtual const char* GetName() override {return TranslatedName.c_str();}

    virtual void OnGUI(Window* window) override {
        World* world = GApplication->GetEngine()->GetWorld();
        if(world == nullptr) {
            return;
        }

        ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_Once);
        ImGui::SetNextWindowBgAlpha(1);
        if(ImGui::Begin(GetName(), 0, ImGuiWindowFlags_NoCollapse)){
            ImGui::Text("%s", world->GetWorldName().c_str());
            
            for(const ObjectRef<Entity>& entity : world->GetEntities()) {
                EntityContainer(entity.Get());
            }
        }
        ImGui::End();
    }
};