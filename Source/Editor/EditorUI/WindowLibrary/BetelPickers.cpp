#include "BetelPickers.h"

#include "BetelColors.h"
#include "BetelImages.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <Engine/Engine.h>

namespace BImGui
{
    extern bool InputString(const char *id, std::string &string, ImGuiInputTextFlags flags = 0);

    namespace Internal {
        std::string _GetObjectNameWithID(uint32_t object, bool *bIsNull = nullptr);
    };
};

std::string BImGui::Internal::_GetObjectNameWithID(uint32_t object, bool *bIsNull)
{
    if(object == 0) {
        if(bIsNull != nullptr) {
            *bIsNull = true;
        }
        return "None";
    }
    
    ObjectLibrary *lib = GEngine->GetObjectLibrary();
    
    ObjectDescriptor* obj = lib->GetObjectDescriptor(object);
    if(obj == nullptr) {
        if(bIsNull != nullptr) {
            *bIsNull = true;
        }
        return std::string("Null (") + std::to_string(object) + ")";
    }
    *bIsNull = false;
    return obj->name + " (" + std::to_string(object) + ")"; 
}

// Return true if value has changed
bool BImGui::InputObject(const char *name, uint32_t &object, std::string &filterString, const char *typeFilter)
{
    ObjectLibrary *lib = GEngine->GetObjectLibrary();

    bool bIsCurrentObjectNull = false;
    std::string CurrentObjectName = Internal::_GetObjectNameWithID(object, &bIsCurrentObjectNull);
    if(bIsCurrentObjectNull) {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors::ErrorRedColor);
    }

    if(ImGui::BeginCombo((std::string("##Combo_") + name).c_str(), CurrentObjectName.c_str())) {
        ImGui::EndCombo();
    }

    if(bIsCurrentObjectNull) {
        ImGui::PopStyleColor();
    }

    if (ImGui::BeginPopupContextItem((std::string("##Combo_") + name).c_str(), ImGuiPopupFlags_MouseButtonLeft))
    {
        BImGui::InputString("Search", filterString);
        for (ObjectDescriptor *desc : lib->GetObjectDescriptorsOfTypeID(typeFilter, true))
        {
            if (ImGui::Selectable((desc->name + " (" + std::to_string(desc->id) + ")").c_str(), (desc->id == object)))
            {
                object = desc->id;
                ImGui::EndPopup();
                return true;
            }
        }
        ImGui::EndPopup();
    }

    ImGui::SameLine();
    
    if(ImGui::ImageButton((std::string("##Button_") + name).c_str(), BImGui::GetEdImage(BImGui::Img::Visibility32Icon), ImVec2(13, 13)))
    {
        object = 0;
        return true;
    }

    ImGui::SameLine();

    ImGui::TextUnformatted(name);

    return false;
}