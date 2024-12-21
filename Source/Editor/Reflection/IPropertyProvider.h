#pragma once

#include <vector>

#include "Property.h"

struct PropertyContainer
{
    std::vector<Property> properties;

    // TODO: autodeducted property pushing
    void PushProperty(std::string name, int32_t *integer) 
    {
        properties.push_back(Property(PropertyType::Integer, name, integer));
    }
};

class IPropertyProvider
{
public:
    virtual PropertyContainer GetEditorReflectedProperties() {return {};}
};