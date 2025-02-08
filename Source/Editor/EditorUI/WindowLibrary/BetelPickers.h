#pragma once
#include <vector>
#include <concepts>
#include <type_traits>
#include <string>

namespace BImGui
{
    // Return true if value has changed
    extern bool InputObject(const char *name, uint32_t &object, std::string &filterString,  const char *typeFilter = "Object");
};