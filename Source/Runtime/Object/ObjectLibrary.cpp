#include "ObjectLibrary.h"

#include <AssetLoader/AssetLoader.h>
#include <Engine/Engine.h>

ObjectDescriptor::~ObjectDescriptor() 
{
    delete object;
    object = nullptr;
}

ObjectLibrary::ObjectLibrary(AssetLoader* assetLoader)
 : loader(assetLoader) 
{
    assert(loader != nullptr && "ObjectLibrary ctor: assetLoader == nullptr");
    
    loader->OnNewObjectID.BindMember(this, &ObjectLibrary::RegisterObjectID);
}

ObjectLibrary::~ObjectLibrary()
{
    
}

void ObjectLibrary::RegisterObjectID(uint32_t ID)
{
    if(objects.contains(ID)) {
        assert(!"Passed Object ID is already registered in the engine");
    }
    objects.emplace(ID, nullptr);
}

void ObjectLibrary::RegisterObjectUsage(uint32_t id) {
    if(!objects.contains(id)) {
        assert(!"Passed Object ID is not registered in the engine");
        return;
    }

    objects[id].usages += 1;

    if(objects[id].usages > 1) {
        return;
    }

    // If usages were 0, but now 1, yet object is not yet unloaded, remove mark
    if(Object* obj = objects[id].object) {
            if(obj->HasFlag(ObjectFlags::Unload)) {
                obj->UnsetFlag(ObjectFlags::Unload);
            }
    }
}

void ObjectLibrary::UnregisterObjectUsage(uint32_t id) {
    if(!objects.contains(id)) {
        assert(!"Passed Object ID is not registered in the engine");
        return;
    }

    assert(objects[id].usages != 0 && "Object is not in use already. This assert indicates object lifecycle malfunction");

    objects[id].usages -= 1;

    if(objects[id].usages != 0) {
        return;
    }

    // If usages == 0, mark object for unloading
    if(Object* obj = objects[id].object) {
            obj->SetFlag(ObjectFlags::Unload);
    }
}

void ObjectLibrary::AddObject(uint32_t id, Object* preloaded)
{
    objects.emplace(id, preloaded);
    if(id > LastObjectID) {
        LastObjectID = id;
    }
}

uint32_t ObjectLibrary::GenerateObjectID() 
{
    return ++LastObjectID;
}

uint32_t ObjectLibrary::GenerateObjectIDSlow()
{
    for(uint32_t id = 1; id < objects.max_size(); ++id)
    {
        if(!objects.contains(id)) {
            return id;
        }
    }

    return ++LastObjectID;
}

Object* ObjectLibrary::LoadObject(uint32_t id)
{
    // If Object is not registered in objects map, it means that it's doesn't exist or is unaccessible
    if(!objects.contains(id)) {
        return nullptr;
    }
    
    // Return object if it's already loaded
    if(objects[id].object != nullptr) {
        return objects[id].object;
    }

    // Load Object
    LoadedObjectDescriptor loadedObject = loader->LoadObject(id);
    
    objects[id].object = loadedObject.object;

    // Reparent Object
    if(loadedObject.object != nullptr && loadedObject.parent != 0)
    {
        // FIXME: Should we load parent if it's not loaded?
        Object* parent = LoadObject(loadedObject.parent);
        if(parent == nullptr) {
            assert(!"Attempt to reparent object to an invalid parent object");
        } else {
            loadedObject.object->Reparent(parent);
        }
    }

    return loadedObject.object;
}

bool ObjectLibrary::IsObjectValid(uint32_t id) const
{
    return objects.contains(id);
}