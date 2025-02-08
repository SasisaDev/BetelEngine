#include "BetelPickers.h"

#include "BetelColors.h"

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
    
    Object* obj = lib->LoadObject(object);
    if(obj == nullptr) {
        if(bIsNull != nullptr) {
            *bIsNull = true;
        }
        return std::string("Null (") + std::to_string(object) + ")";
    }
    *bIsNull = false;
    return obj->GetName() + " (" + std::to_string(object) + ")"; 
}

// Return true if value has changed
bool BImGui::InputObject(const char *name, uint32_t &object, std::string &filterString, const char *typeFilter)
{
    ObjectLibrary *lib = GEngine->GetObjectLibrary();

    bool bIsCurrentObjectNull = false;
    std::string CurrentObjectName = Internal::_GetObjectNameWithID(123, &bIsCurrentObjectNull);
    if(bIsCurrentObjectNull) {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors::ErrorRedColor);
    }

    if(ImGui::BeginCombo(name, CurrentObjectName.c_str())) {
        ImGui::EndCombo();
    }
    
    if(bIsCurrentObjectNull) {
        ImGui::PopStyleColor();
    }

    if (ImGui::BeginPopupContextItem(name, ImGuiPopupFlags_MouseButtonLeft))
    {
        BImGui::InputString("Search", filterString);
        for (ObjectDescriptor *desc : lib->GetObjectDescriptorsOfTypeID(typeFilter, true))
        {
            if (ImGui::Selectable((desc->name + " (" + std::to_string(desc->id) + ")").c_str(), (desc->id == object)))
            {
                object = desc->id;
                return true;
            }
        }
        ImGui::EndPopup();
    }

    return false;
}