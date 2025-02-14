#pragma once

#include <memory>
#include <string>
#include <cstdint>

enum class PropertyType : uint8_t {
    Integer,
    String,
    Text,
    Boolean,
    Float,
    Object,
    // Used to call a function by pressing a button in Property Editor
    Method,
};

struct Property 
{
    PropertyType type;
    std::string_view name;
    void* value;

    Property(){}
    Property(PropertyType t, const std::string_view& n, void* v){type = t; name = n; value = v;}
};