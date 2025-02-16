#include "BetelInputs.h"

#include "BetelColors.h"
#include "BetelImages.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <Engine/Engine.h>
#include <Object/ObjectLibrary.h>

namespace BImGui
{
    int StringResizeCallback(ImGuiInputTextCallbackData* data)
    {
        std::string* str = (std::string*)data->UserData;
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            IM_ASSERT(str->data() == data->Buf);
            // Resulting string should be one byte smaller,
            // Since ImGui provides null-terminated string in buffer
            // And we don't accept it in string
            str->resize(data->BufTextLen); 
            data->Buf = str->data();
        } else if(data->EventFlag == ImGuiInputTextFlags_CallbackEdit) {
            memcpy(str->data(), data->Buf, data->BufTextLen);
        }
        return 0;
    }

    bool InputString(const char* id, std::string& string, ImGuiInputTextFlags flags)
    {
        return ImGui::InputText(id, string.data(), (size_t)string.size(), flags | ImGuiInputTextFlags_CallbackResize, BImGui::StringResizeCallback, (void*)(&string));
    }

    namespace Internal {
        std::string _GetObjectNameWithID(uint32_t object, bool *bIsNull = nullptr)
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
    };

    // Return true if value has changed
    bool InputObject(const char *name, uint32_t &object, std::string &filterString, const char *typeFilter)
    {
        assert(strlen(name) > 0 && "Input name is too short");

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

        if(ImGui::ImageButton((std::string("##Button_") + name).c_str(), BImGui::GetEdImage(BImGui::Img::Cross32Icon), ImVec2(13, 13)))
        {
            object = 0;
            return true;
        }

        if(strlen(name) >= 2 && !(name[0] == '#' || name[1] == '#')){
            ImGui::SameLine();
            ImGui::TextUnformatted(name);
        }

        return false;
    }

};