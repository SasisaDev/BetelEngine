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

};

struct Property 
{
    PropertyType type;
    std::string name;
    void* value;

    Property(){}
    Property(PropertyType t, std::string n, void* v){type = t; name = n; value = v;}
};