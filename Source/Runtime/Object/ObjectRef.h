#pragma once

#include <Log/Logger.h>
#include "ObjectConcept.h"
#include "ObjectLibrary.h"

class Object;

template <ObjectClass _ObjectT>
class ObjectRef
{
    virtual void InternalRegisterUsage()
    {
        // TODO: Uncomment this once Library system is fully integrated
        //ObjectLibrary::Get().RegisterObjectUsage(objectID);
    }
    virtual void InternalUnregisterUsage()
    {
        //ObjectLibrary::Get().UnregisterObjectUsage(objectID);
    }
protected:
    _ObjectT* ref = nullptr;
    uint32_t objectID = 0;

public:
    ObjectRef() {}
    ObjectRef(ObjectRef<_ObjectT> &otherObjectRef) {
        ref = otherObjectRef.ref;
        objectID = otherObjectRef.objectID;
        if(objectID) {
            InternalRegisterUsage();
        }
    }
    ObjectRef(uint32_t id) : objectID(id) {InternalRegisterUsage();}
    ObjectRef(_ObjectT* object) : ref(object) 
    {
        if(ref != nullptr) {
            objectID = ref->GetID();
            InternalRegisterUsage();
        }
    }
    ~ObjectRef() {InternalUnregisterUsage();}

    inline bool IsValid() const;
    inline bool IsLoaded() const {return ref;}

    inline _ObjectT* Get() const {return ref;} 
    inline uint32_t GetID() const {return objectID;} 
    _ObjectT* Load();
};

template <ObjectClass _ObjectT>
class WeakObjectRef : public ObjectRef<_ObjectT>
{
    virtual void InternalRegisterUsage() override {return;} 
    virtual void InternalUnregisterUsage() override {return;} 
};