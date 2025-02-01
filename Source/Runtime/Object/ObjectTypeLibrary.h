#pragma once

#include <map>
#include <unordered_map>
#include <cassert>

#include "Object.h"
#include "ObjectConcept.h"

class ObjectTypeLibrary
{
    std::unordered_map<std::string, ObjectType*> objectTypes;
public:

    static ObjectTypeLibrary& Get() {
        static ObjectTypeLibrary lib;
        return lib;
    }

    template <ObjectTypeClass _ObjectTypeT>
    bool RegisterObjectType(const std::string& typeName) {
        assert(typeName.size() > 0 && "Object Type Name's length must be longer than 0");
        assert(objectTypes.contains(typeName) == false && "Object Type with this name is already registered");
        objectTypes[typeName] = new _ObjectTypeT;
        return true;
    }

    inline ObjectType* GetObjectType(const std::string& Name) {
        if(objectTypes.contains(Name))
        {
            return objectTypes[Name];
        }
        return nullptr;
    }

    inline const std::unordered_map<std::string, ObjectType*>& GetObjectTypes() {
        return objectTypes;
    }
};