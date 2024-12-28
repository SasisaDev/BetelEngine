#pragma once

#include <map>
#include <unordered_map>
#include <cassert>

#include "Object.h"
#include "ObjectConcept.h"

struct ObjectDescriptor
{
    Object* object;
    uint32_t usages;
};

class ObjectLibrary
{
    template <ObjectClass _ObjectT>    
    friend class ObjectRef;

    void RegisterObjectUsage(uint32_t id);
    void UnregisterObjectUsage(uint32_t id);
protected:
    std::unordered_map<uint32_t, ObjectDescriptor> objects;
    std::unordered_map<std::string, ObjectType*> objectTypes;
public:

    static ObjectLibrary& Get() {
        static ObjectLibrary lib;
        return lib;
    }

    Object* LoadObject(uint32_t id);
    bool IsObjectValid(uint32_t id) const;

    template <ObjectTypeClass _ObjectTypeT>
    bool RegisterObjectType(std::string typeName) {
        assert(typeName.size() > 0 && "Object Type Name's length must be longer than 0");
        objectTypes[typeName] = new _ObjectTypeT;
        return true;
    }
};