#include "Object.h"

#include <algorithm>
#include <Utility/StrSan.h>

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
    Name = StringSanitizer::Sanitize(newName);
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
    return container;
}
#endif