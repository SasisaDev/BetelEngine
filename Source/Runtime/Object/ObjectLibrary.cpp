#include "ObjectLibrary.h"

#include <AssetLoader/AssetLoader.h>
#include <Engine/Engine.h>
#include "ObjectTypeLibrary.h"

std::string ObjectLibrary::ObjectName_None = "None";

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
    AddObject(ID, nullptr);

#   ifdef EDITOR
    LoadedObjectMetadata metadata = loader->LoadObjectMetadata(ID);
    objects[ID].id = ID;
    objects[ID].name = metadata.name;
    objects[ID].type = metadata.type;
#   endif
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
                obj->ClearFlag(ObjectFlags::Unload);
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

    AddObject(objectID, object);
    return object;
}

void ObjectLibrary::AddObject(uint32_t id, Object* preloaded)
{
    objects.emplace(id, preloaded);

    if(id > LastObjectID) {
        LastObjectID = id;
    }

#   ifdef EDITOR
    UpdateObjectDescriptorMetadata(id);
#   endif
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
            // TODO: Check inheritance

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

#ifdef EDITOR
void ObjectLibrary::LoadAllObjects()
{
    for(auto it = objects.begin(); it != objects.end(); ++it)
    {
        LoadObject(it->first);
    }
}

std::vector<ObjectDescriptor*> ObjectLibrary::GetAllObjectDescriptors(bool excludeTransient)
{
    std::vector<ObjectDescriptor*> objs;
    objs.reserve(16);

    for(auto it = objects.begin(); it != objects.end(); ++it)
    {
        const bool bIsTransient = (it->second.object != nullptr) ? it->second.object->HasFlag(ObjectFlags::Transient) : false;
        if((excludeTransient) ? !bIsTransient : true)
        {
            objs.push_back(&(it->second));
        }
    }

    return objs;
}

std::vector<ObjectDescriptor*> ObjectLibrary::GetObjectDescriptorsOfTypeID(const std::string& typeID, bool excludeTransient)
{
    std::vector<ObjectDescriptor*> objs;
    objs.reserve(16);

    for(auto it = objects.begin(); it != objects.end(); ++it)
    {
        const bool bSameType = it->second.type == typeID;
        const bool bIsTransient = (it->second.object != nullptr) ? it->second.object->HasFlag(ObjectFlags::Transient) : false;
        const bool bNotTransient = (excludeTransient) ? !bIsTransient : true;
        if( bSameType && bNotTransient)
        {
            // TODO: Check inheritance

            objs.push_back(&(it->second));
        }
    }

    return objs;
}

ObjectDescriptor* ObjectLibrary::GetObjectDescriptor(uint32_t ID)
{
    if(!objects.contains(ID)) {
        return nullptr;
    }

    return &objects[ID];
}

void ObjectLibrary::UpdateObjectDescriptorMetadata(uint32_t id)
{
    objects[id].id = id;
    if(objects[id].object)
    {
        objects[id].name = objects[id].object->GetName();
        objects[id].type = objects[id].object->GetType();
    }
}
#endif