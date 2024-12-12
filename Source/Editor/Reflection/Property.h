#pragma once

#include <memory>

enum class PropertyType {
    
};

struct Property 
{
    PropertyType type;
    void* value;

    Property(){}
    Property(PropertyType t, void* v){type = t; value = v;}
};