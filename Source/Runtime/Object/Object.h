#pragma once

#include <string>
#include <vector>

#ifdef EDITOR
#   include <Reflection/IPropertyProvider.h>
#endif

class SerialArchive;

class Object
#ifdef EDITOR
    : public IPropertyProvider
#endif
{
protected:
    std::string Name;
    std::string Path;
    
    std::vector<Object*> Children;
    Object* Parent;
public:

    void Reparent(Object* newParent);
    void Rename(std::string newName);

    inline std::string GetName() const {return Name;}
    inline std::string GetPath() const {return Path;}

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