#pragma once

#include <imgui/imgui_internal.h>
#include "BetelColors.h"

namespace BImGui {

bool ImageButton(const char* ID, ImTextureID ImgID, ImVec2 ImgSize, ImVec2 Uv0 = ImVec2(0, 0), ImVec2 Uv1 = ImVec2(1, 1)) {
    return ImGui::ImageButton(ID, ImgID, ImgSize, Uv0, Uv1);
}

bool ToggleImageButton(const char* ID, bool* value, ImTextureID ImgID, ImVec2 ImgSize, 
                        ImVec4 Color = BImGui::Colors::UnselectedToggleColor, ImVec4 ActiveColor = BImGui::Colors::ContrastColor, 
                        ImVec2 Uv0 = ImVec2(0, 0), ImVec2 Uv1 = ImVec2(1, 1)) 
{
    if(*value){
        ImGui::PushStyleColor(ImGuiCol_Button, ActiveColor);
    } else {
        ImGui::PushStyleColor(ImGuiCol_Button, Color);
    }
    if(ImGui::ImageButton(ID, ImgID, ImgSize, Uv0, Uv1)) {
        *value = !*value;
        ImGui::PopStyleColor();
        return true;
    }
    ImGui::PopStyleColor();
    return false;
}

};