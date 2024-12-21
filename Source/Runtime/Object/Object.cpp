#include "Object.h"

PropertyContainer Object::GetEditorReflectedProperties()
{
    PropertyContainer container;

    container.PushProperty("Test", new int);

    return container;
}