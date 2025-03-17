#pragma once

#include <Log/Logger.h>
#include "ObjectConcept.h"
#include "ObjectLibrary.h"

class Object;

template <ObjectClass _ObjectT>
class ObjectRef
{
    void InternalRegisterUsage();
    void InternalUnregisterUsage();
protected:
    _ObjectT* ref = nullptr;
    uint32_t objectID = 0;

public:
    ObjectRef() { /*InternalRegisterUsage();*/ }
    // Copy constructor
    ObjectRef(const ObjectRef<_ObjectT> &otherObjectRef) {
        ref = otherObjectRef.ref;
        objectID = otherObjectRef.objectID;
        InternalRegisterUsage();
    }
    // Move Constructor
    ObjectRef(ObjectRef<_ObjectT> &&otherObjectRef)
        : ref(std::move(otherObjectRef.ref)), objectID(std::move(otherObjectRef.objectID))
    {
        InternalRegisterUsage();
    }
    ObjectRef(uint32_t id) : objectID(id) {InternalRegisterUsage();}
    ObjectRef(_ObjectT* object) : ref(object) 
    {
        if(ref != nullptr) {
            objectID = ref->GetID();
            InternalRegisterUsage();
        }
    }
    ~ObjectRef() { 
        InternalUnregisterUsage();
    }

    inline bool IsValid() const;
    inline bool IsLoaded() const {return ref;}

    void Reset(uint32_t newID);

    inline _ObjectT* Get() const {return ref;} 
    inline uint32_t GetID() const {return objectID;} 
    _ObjectT* Load();

    inline _ObjectT* operator ->() const {return ref;}  
    inline operator _ObjectT*() const {return ref;}
    bool operator ==(const ObjectRef<_ObjectT>& other) {return other.ref == ref && other.objectID == objectID;}  

    // Move operator
    constexpr ObjectRef<_ObjectT>& operator=(ObjectRef<_ObjectT>&& other)
    {
        InternalUnregisterUsage();

        ref = std::move(other.ref);
        objectID = std::move(other.objectID);

        InternalRegisterUsage();

        return *this;
    }

    // Copy operator
    constexpr ObjectRef<_ObjectT>& operator=(const ObjectRef<_ObjectT>& other)
    {
        InternalUnregisterUsage();

        ref = other.ref;
        objectID = other.objectID;

        InternalRegisterUsage();

        return *this;
    }
};

#include <Engine/Engine.h>

template <ObjectClass _ObjectT>
inline void ObjectRef<_ObjectT>::InternalRegisterUsage() 
{
    // FIXME: These checks aren't required if we destroy ObjectLibrary last and avoid singletons
    if(GEngine != nullptr && objectID)
        GEngine->GetObjectLibrary()->RegisterObjectUsage(objectID);
}

template <ObjectClass _ObjectT>
inline void ObjectRef<_ObjectT>::InternalUnregisterUsage() 
{
    if(GEngine != nullptr && objectID)
        GEngine->GetObjectLibrary()->UnregisterObjectUsage(objectID);
}

template <ObjectClass _ObjectT>
inline bool ObjectRef<_ObjectT>::IsValid() const 
{
    return GEngine->GetObjectLibrary()->IsObjectValid(objectID);
}

template <ObjectClass _ObjectT>
inline void ObjectRef<_ObjectT>::Reset(uint32_t newID)
{
    InternalUnregisterUsage();

    ref = nullptr;
    objectID = newID;

    InternalRegisterUsage();
}

template <ObjectClass _ObjectT>
_ObjectT* ObjectRef<_ObjectT>::Load() 
{
    if(ref != nullptr) 
        return ref;
    
    _ObjectT* obj = dynamic_cast<_ObjectT*>(GEngine->GetObjectLibrary()->LoadObject(objectID));
    ref = obj;
    return obj;
}