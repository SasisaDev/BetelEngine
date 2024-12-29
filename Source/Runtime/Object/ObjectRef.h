#pragma once

#include <Log/Logger.h>
#include "ObjectConcept.h"
#include "ObjectLibrary.h"

class Object;

// TODO
template <ObjectClass _ObjectT>
class ObjectRef
{
    virtual void InternalRegisterUsage()
    {
        ObjectLibrary::Get().RegisterObjectUsage(objectID);
    }
protected:
    _ObjectT* ref = nullptr;
    uint32_t objectID = 0;

public:

    inline bool IsValid() const {return ObjectLibrary::Get().IsObjectValid(objectID);} 
    inline bool IsLoaded() const {return ref;}

    _ObjectT* Get() const {return ref;} 
    uint32_t GetID() const {return objectID;} 
    _ObjectT* Load() 
    {
        _ObjectT* obj = dynamic_cast<_ObjectT*>(ObjectLibrary::Get().LoadObject(objectID));
        return obj;
    }
};

template <ObjectClass _ObjectT>
class WeakObjectRef : public ObjectRef<_ObjectT>
{
    virtual void InternalRegisterUsage() override {return;} 
};