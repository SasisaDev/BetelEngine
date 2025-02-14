#pragma once

#include <imgui/imgui_internal.h>
#include "BetelColors.h"

#include <string>

namespace BImGui {

    extern bool InputString(const char* id, std::string& string, ImGuiInputTextFlags flags = 0);

    // Return true if value has changed
    extern bool InputObject(const char *name, uint32_t &object, std::string &filterString,  const char *typeFilter = "Object");

};