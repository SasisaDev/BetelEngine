#pragma once

#include <map>
#include <unordered_map>
#include <cassert>
#include <string>

#include "Resource.h"

// TODO
// MAYBE REMOVE?

struct ResourceDescriptor
{
    Resource* resource;
    uint32_t usages;
};

class ResourceLibrary
{
protected:
    std::unordered_map<std::string, ResourceDescriptor> resources;
public:

    static ResourceLibrary& Get() {
        static ResourceLibrary lib;
        return lib;
    }
};