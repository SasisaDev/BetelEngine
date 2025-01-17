#pragma once

#include <map>
#include <unordered_map>
#include <cassert>

#include "Object.h"
#include "ObjectConcept.h"

struct ObjectDescriptor
{
    Object* object = nullptr;
    uint32_t usages = 0;

    ObjectDescriptor(Object* objPtr) {object = objPtr;}
    ObjectDescriptor() {}
};

class ObjectLibrary
{
    template <ObjectClass _ObjectT>    
    friend class ObjectRef;

    void RegisterObjectUsage(uint32_t id);
    void UnregisterObjectUsage(uint32_t id);

    uint32_t LastObjectID = 1;
    uint32_t GenerateObjectID();
    // Generates unique Object ID, taking holes in a map into an account. It's way slower, but creates denser distribution
    uint32_t GenerateObjectIDSlow();
protected:
    friend class AssetLoader;

    std::unordered_map<uint32_t, ObjectDescriptor> objects;

    void AddObject(uint32_t id, Object* preloaded = nullptr);
public:

    template <ObjectClass _ObjectT>
    _ObjectT* CreateObject(std::string Name, bool Transient = true) {
        uint32_t objectID = GenerateObjectID();
        _ObjectT* object = new _ObjectT();
        object->SetID(objectID);
        object->Rename(Name);

        // Set Transient flag
        if(Transient) {
            object->SetFlag(ObjectFlags::Transient);
        }

        objects[objectID] = ObjectDescriptor(object);
        return object;
    }

    Object* LoadObject(uint32_t id);
    bool IsObjectValid(uint32_t id) const;
    // This function should only be called in GC Pass, when object is not used anymore
    Object* UnloadObject(uint32_t id);
    /* 
     * Calling this function will unload object and destroy it completely from the database 
     * It may result in broken pointers, use with caution.
    */
    Object* DestroyObject(uint32_t id);
};