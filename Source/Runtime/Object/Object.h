#pragma once

#include <string>
#include <vector>
#include <cstdint>

#include "FieldContainer.h"

#ifdef EDITOR
#   include <Reflection/IPropertyProvider.h>
#endif

class ObjectType;

/*
 * Loaded objects may contain these flags, that define their runtime behaviour
 */
enum class ObjectFlags : uint8_t {
    // This object is created only for runtime use, do not save to disk. Destroy permamently once parent is unloaded
    Transient = (1 << 0),
    // Unload on next GC Pass. If set, should not be referenced or used by anything
    Unload = (1<<1),
    // This object was changed and needs changes to be saved (Save System or Asset System) 
    Dirty = (1<<2),
    // Marks this object undestructable by GC
    BypassGC = (1<<3),
};

/*
 * Objects are the main data containers inside the engine
 * Each object has it's unique 32bit ID, that must be consistent between all inner systems
 * Alongside Name string, which is used by Editor and debug features to introduce user friendly naming
 * 
 * Automated behaviours of Object can be changed by setting flags
 * 
 * Each Object can contain multiple Children, as well as own Parent object  
 * 
 * Object ID 0 is reserved for None; 
 */
class Object
#ifdef EDITOR
    : public IPropertyProvider
#endif
{
protected:
    uint8_t Flags = 0;
    uint32_t ID = 0;
    std::string Name = "";
    
    std::vector<Object*> Children;
    Object* Parent = nullptr;
public:
    Object() {}
    Object(uint32_t id) {ID = id;}
    virtual ~Object(){}

    void Reparent(Object* newParent);
    void Rename(std::string newName);
    // TODO: Maybe make this private? It can cause big issues
    void SetID(uint32_t newID) {ID = newID;}

    inline uint32_t GetID() const {return ID;}
    inline const std::string& GetName() const {return Name;}
    /*
     * Returns this object's type
     * Must be overriden in every new Object class
    */
    static std::string GetType() {return "OBJ";}

    void SetFlag(ObjectFlags flag) {Flags |= (uint8_t)flag;}
    void UnsetFlag(ObjectFlags flag) {Flags &= ~(uint8_t)flag;}
    bool HasFlag(ObjectFlags flag) {return (Flags & (uint8_t)flag) == (uint8_t)flag;}

    // Marks object Dirty, meaning that the object has changed and should be saved again
    void Dirty() {Flags |= (uint8_t)ObjectFlags::Dirty;}

    template <typename ChildObjectT = Object>
    inline std::vector<ChildObjectT*> GetChildren() const {
        std::vector<ChildObjectT*> typedChildrenArray {};
        for(Object* child : Children)
        {
            if(ChildObjectT* typedChild = dynamic_cast<ChildObjectT*>(child))
            {
                typedChildrenArray.push_back(typedChild);
            }
        }
        return Children;
    }
    
    template <typename ParentObjectT = Object>
    ParentObjectT* GetParent() 
    {
        // If parent doesn't match the template type, attempt to find next parent
        if(dynamic_cast<ParentObjectT*>(Parent) == nullptr && Parent != nullptr) {
            return Parent->GetParent<ParentObjectT>();
        } else {
            return dynamic_cast<ParentObjectT*>(Parent);
        }
    }
    
    virtual void Serialize(FieldContainer& container){}
    
    //~Reflection API start
#ifdef EDITOR 
    virtual PropertyContainer GetEditorReflectedProperties() override;
#endif
    //~Reflection API stop
};

class ObjectType
{
public:
    virtual Object* CreateInstance() { return new Object; }
    virtual bool ShowInEditor() { return true; }
};