#pragma once

#include <memory>
#include <functional>
#include <unordered_map>
#include <string>

#include <Log/Logger.h>

class EditorToolkitWindow;
class Object;

class ObjectEditorViewsFactory
{
    static std::unordered_map<std::string, std::function<std::unique_ptr<EditorToolkitWindow>(Object*)>> FactoryConstructors;
public:
    /*
     * This function allows custom registration of edit views constructors
     * It also allows redefinition of existing constructors 
     */
    static bool EmplaceConstructor(std::string ID, const std::function<std::unique_ptr<EditorToolkitWindow>(Object*)>& Constructor) {
        FactoryConstructors.emplace(ID, Constructor);
    }

    static std::unique_ptr<EditorToolkitWindow> CreateEditView(const std::string& ObjectTypeID, Object* obj)
    {
        if(FactoryConstructors.contains(ObjectTypeID))
        {
            return FactoryConstructors[ObjectTypeID](obj);
        } 
        else
        {
            LOGF(Warning, LogObjectEditViewsFactory, "Couldn't create Edit View for object type \"%s\". Not found!", ObjectTypeID.c_str());
            return nullptr;
        }
    }
};