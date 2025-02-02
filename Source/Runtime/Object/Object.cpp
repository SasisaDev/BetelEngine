#include "Object.h"

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


#ifdef EDITOR 
PropertyContainer Object::GetEditorReflectedProperties()
{
    PropertyContainer container;

    container.PushProperty("Test", new int);

    return container;
}
#endif