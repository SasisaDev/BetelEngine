#pragma once

#include <vector>

#include "Property.h"

class IPropertyProvider
{
public:
    virtual std::vector<Property> GetEditorReflectedProperties() {return {};}
};