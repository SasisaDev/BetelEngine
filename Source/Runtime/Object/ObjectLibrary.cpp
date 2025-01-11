#include "ObjectLibrary.h"

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

uint32_t ObjectLibrary::GenerateObjectID() 
{
    return ++LastObjectID;
}

Object* ObjectLibrary::LoadObject(uint32_t id)
{
    // TODO Load Object
    return nullptr;
}

bool ObjectLibrary::IsObjectValid(uint32_t id) const
{
    return objects.contains(id);
}