#include "ObjectRef.h"
#include "ObjectLibrary.h"
#include <Engine/Engine.h>

template <ObjectClass _ObjectT>
inline bool ObjectRef<_ObjectT>::IsValid() const 
{
    return ObjectLibrary::Get().IsObjectValid(objectID);
}

template <ObjectClass _ObjectT>
_ObjectT* ObjectRef<_ObjectT>::Load() 
{
    _ObjectT* obj = dynamic_cast<_ObjectT*>(GEngine->GetObjectLibrary().LoadObject(objectID));
    return obj;
}