#include "ObjectLibrary.h"

#include <AssetLoader/AssetLoader.h>

void ObjectLibrary::RegisterObjectUsage(uint32_t id) {
    if(!objects.contains(id)) {
        assert(!"Passed Object ID is not registered in the engine");
        return;
    }

    objects[id].usages += 1;
}

void ObjectLibrary::UnregisterObjectUsage(uint32_t id) {
    if(!objects.contains(id)) {
        assert(!"Passed Object ID is not registered in the engine");
        return;
    }

    if(objects[id].usages == 0) {
        assert(!"Object is not in use already. This assert indicates object lifecycle malfunction");
        return;
    }

    objects[id].usages -= 1;
}

void ObjectLibrary::AddObject(uint32_t id, Object* preloaded)
{
    objects[id] = {preloaded};
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
    for(uint32_t id = 0; id < objects.max_size(); ++id)
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
    Object* loadedObject = AssetLoader::Get().LoadObject(id);
    
    objects[id].object = loadedObject;

    return loadedObject;
}

bool ObjectLibrary::IsObjectValid(uint32_t id) const
{
    return objects.contains(id);
}