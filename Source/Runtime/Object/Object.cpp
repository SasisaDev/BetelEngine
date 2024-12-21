#include "Object.h"

void Object::Reparent(Object* newParent)
{
    Parent = newParent;
}

void Object::Rename(std::string newName)
{
    Name = newName;
}

PropertyContainer Object::GetEditorReflectedProperties()
{
    PropertyContainer container;

    container.PushProperty("Test", new int);

    return container;
}