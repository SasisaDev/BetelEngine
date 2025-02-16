#pragma once
#include <vector>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace BImGui
{
    extern bool IconSelectableID(const char* name, const char* id, ImTextureID tex, bool selected = false);
};