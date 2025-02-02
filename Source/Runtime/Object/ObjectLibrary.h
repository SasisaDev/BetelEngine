#pragma once

#include <map>
#include <unordered_map>
#include <cassert>
#include <memory>
#include <atomic>

#include "Object.h"
#include "ObjectConcept.h"

class AssetLoader;

struct ObjectDescriptor
{
    Object *object;
    std::atomic_uint32_t usages = 0;

    ObjectDescriptor(Object* objPtr) {object = objPtr;}
    ObjectDescriptor() {}
    ~ObjectDescriptor();
};

class ObjectLibrary
{
    template <ObjectClass _ObjectT>    
    friend class ObjectRef;

    void RegisterObjectUsage(uint32_t id);
    void UnregisterObjectUsage(uint32_t id);

    uint32_t LastObjectID = 0;
    uint32_t GenerateObjectID();
    // Generates unique Object ID, taking holes in a map into an account. It's way slower, but creates denser distribution
    uint32_t GenerateObjectIDSlow();

    void RegisterObjectID(uint32_t ID);

    AssetLoader *loader;
protected:
    std::unordered_map<uint32_t, ObjectDescriptor> objects;

    void AddObject(uint32_t id, Object* preloaded = nullptr);
public:
    ObjectLibrary(AssetLoader* assetLoader);
    ~ObjectLibrary();

    template <ObjectClass _ObjectT>
    _ObjectT* CreateObject(const std::string& Name, bool Transient = true) {
        uint32_t objectID = GenerateObjectID();
        _ObjectT* object = new _ObjectT();
        object->SetID(objectID);
        object->Rename(Name);
        object->Type = _ObjectT::GetStaticType();

        // Set Transient flag
        if(Transient) {
            object->SetFlag(ObjectFlags::Transient);
        }

        objects.emplace(objectID, object);
        return object;
    }

    Object* CreateObjectFromTypeID(const std::string& TypeID, const std::string& Name, bool Transient = true);

    Object* LoadObject(uint32_t id);
    bool IsObjectValid(uint32_t id) const;
    // This function should only be called in GC Pass, when object is not used anymore
    Object* UnloadObject(uint32_t id);
    /* 
     * Calling this function will unload object and destroy it completely from the database 
     * It may result in broken pointers, use with caution.
    */
    void DestroyObject(uint32_t id);

    /*
     * Goes through all registered objects and returns a vector of pointers to loaded objects 
     *
     * WARNING: Very heavy function, use with care!
     */
    std::vector<Object*> GetAllObjects(bool excludeTransient = false);

    /*
     * Goes through all registered objects and returns a vector of objects with specified Type ID 
     * 
     * At Runtime will only go through currently loaded objects
     * 
     * WARNING: Very heavy function, use with care!
     */
    std::vector<Object*> GetObjectsOfTypeID(const std::string& typeID, bool excludeTransient = false);
};