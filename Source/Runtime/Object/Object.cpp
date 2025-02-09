#include "Object.h"

#include <algorithm>

#ifdef EDITOR
#   include <Engine/Engine.h>
#   include "ObjectLibrary.h"
#endif

void Object::Reparent(Object* newParent)
{
    Parent = newParent;
}

void Object::Rename(const std::string& newName)
{
    Name = newName;
    // Remove all null terminators from the string
    Name.erase(std::find(Name.begin(), Name.end(), '\0'), Name.end());
}

void Object::Dirty() 
{
    Flags |= (uint8_t)ObjectFlags::Dirty;

#   ifdef EDITOR
    GEngine->GetObjectLibrary()->UpdateObjectDescriptorMetadata(ID);
#   endif
}

#ifdef EDITOR 
PropertyContainer Object::GetEditorReflectedProperties()
{
    PropertyContainer container;

    container.PushProperty("Test", new int);

    return container;
}
#endif