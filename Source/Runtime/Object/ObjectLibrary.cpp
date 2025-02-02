#include "ObjectLibrary.h"

#include <AssetLoader/AssetLoader.h>
#include <Engine/Engine.h>
#include "ObjectTypeLibrary.h"

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

Object* ObjectLibrary::CreateObjectFromTypeID(const std::string& TypeID, const std::string& Name, bool Transient)
{
    // Find Type Factory in ObjectTypeLibrary
    ObjectType* type = ObjectTypeLibrary::Get().GetObjectType(TypeID);
    if(type == nullptr) {
        return nullptr;
    }

    uint32_t objectID = GenerateObjectID();
    Object* object = type->CreateInstance();
    object->SetID(objectID);
    object->Rename(Name);
    object->Type = TypeID;

    // Set Transient flag
    if(Transient) {
        object->SetFlag(ObjectFlags::Transient);
    }

    objects.emplace(objectID, object);
    return object;
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

    objects[id].object->Type = loadedObject.type;

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

std::vector<Object*> ObjectLibrary::GetAllObjects(bool excludeTransient)
{
    std::vector<Object*> objs;
    objs.reserve(16);

    for(auto it = objects.begin(); it != objects.end(); ++it)
    {
        if(it->second.object && (excludeTransient) ? !it->second.object->HasFlag(ObjectFlags::Transient) : true)
        {
            objs.push_back(it->second.object);
        }
    }

    return objs;
}

std::vector<Object*> ObjectLibrary::GetObjectsOfTypeID(const std::string& typeID, bool excludeTransient)
{
    std::vector<Object*> objs;
    objs.reserve(16);

    for(auto it = objects.begin(); it != objects.end(); ++it)
    {
        const bool SameType = it->second.object->GetType() == typeID;
        const bool NotTransient = (excludeTransient) ? !it->second.object->HasFlag(ObjectFlags::Transient) : true;
        if( it->second.object 
            && SameType 
            && NotTransient)
        {
            objs.push_back(it->second.object);
        }
    }

    return objs;
}

void ObjectLibrary::DestroyObject(uint32_t id)
{
    if(!objects.contains(id)) {
        return;
    }

    // TODO: Destroy Object safely
    objects.erase(id);
}