#pragma once

#include <Log/Logger.h>
#include "ObjectConcept.h"
#include "ObjectLibrary.h"

class Object;

template <ObjectClass _ObjectT>
class ObjectRef
{
    virtual void InternalRegisterUsage();
    virtual void InternalUnregisterUsage();
protected:
    _ObjectT* ref = nullptr;
    uint32_t objectID = 0;

public:
    ObjectRef() {}
    ObjectRef(const ObjectRef<_ObjectT> &otherObjectRef) {
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

#include <Engine/Engine.h>

template <ObjectClass _ObjectT>
class WeakObjectRef : public ObjectRef<_ObjectT>
{
    virtual void InternalRegisterUsage() override {return;} 
    virtual void InternalUnregisterUsage() override {return;} 
};

template <ObjectClass _ObjectT>
inline void ObjectRef<_ObjectT>::InternalRegisterUsage() 
{
    // FIXME: These checks aren't required if we destroy ObjectLibrary last and avoid singletons
    if(GEngine != nullptr)
        GEngine->GetObjectLibrary()->RegisterObjectUsage(objectID);
}

template <ObjectClass _ObjectT>
inline void ObjectRef<_ObjectT>::InternalUnregisterUsage() 
{
    if(GEngine != nullptr)
        GEngine->GetObjectLibrary()->UnregisterObjectUsage(objectID);
}

template <ObjectClass _ObjectT>
inline bool ObjectRef<_ObjectT>::IsValid() const 
{
    return GEngine->GetObjectLibrary()->IsObjectValid(objectID);
}

template <ObjectClass _ObjectT>
_ObjectT* ObjectRef<_ObjectT>::Load() 
{
    _ObjectT* obj = dynamic_cast<_ObjectT*>(GEngine->GetObjectLibrary()->LoadObject(objectID));
    return obj;
}