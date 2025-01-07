#pragma once

#include <imgui/imgui_internal.h>
#include "BetelColors.h"

#include <string>

namespace BImGui {

    int StringResizeCallback(ImGuiInputTextCallbackData* data)
    {
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            std::string* str = (std::string*)data->UserData;
            IM_ASSERT(str->data() == data->Buf);
            str->resize(data->BufSize); 
            data->Buf = str->data();
        }
        return 0;
    }

    bool InputString(const char* id, std::string& string, ImGuiInputTextFlags flags = 0) {
        return ImGui::InputText(id, string.data(), (size_t)string.size(), flags | ImGuiInputTextFlags_CallbackResize, BImGui::StringResizeCallback, (void*)(&string));
    }

};