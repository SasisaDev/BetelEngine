#pragma once

#include <string>
#include <vector>
#include <cstdint>

#ifdef EDITOR
#   include <Reflection/IPropertyProvider.h>
#endif

class SerialArchive;

enum class ObjectFlags : uint8_t {
    // This object is created only for runtime use, do not save to disk. Destroy permamently once parent is unloaded
    Transient = (1 << 0),
    // Unload on next GC Pass. If set, should not be referenced or used by anything
    Unload = (1<<1),
    // This object was changed and needs changes to be saved (Save System or Asset System) 
    Dirty = (1<<2),
};

class Object
#ifdef EDITOR
    : public IPropertyProvider
#endif
{
protected:
    uint8_t Flags = 0;
    uint32_t ID;
    std::string Name;
    
    std::vector<Object*> Children;
    Object* Parent = nullptr;
public:
    Object() {}
    Object(uint32_t id) {ID = id;}

    void Reparent(Object* newParent);
    void Rename(std::string newName);
    // TODO: Maybe make this closed? It can cause big issues
    void SetID(uint32_t newID) {ID = newID;}

    inline uint32_t GetID() const {return ID;}
    inline std::string GetName() const {return Name;}

    void SetFlag(ObjectFlags flag) {Flags |= (uint8_t)flag;}

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

    virtual void Serialize(SerialArchive& archive){}
    
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