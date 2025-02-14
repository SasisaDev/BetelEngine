#pragma once

#include <vector>
#include <Delegate/Delegate.h>
#include <Object/ObjectConcept.h>

#include "Property.h"

class Text;
template <ObjectClass _ObjectT>
class ObjectRef;

struct PropertyData_Object
{
    Delegate<uint32_t> GetID;
    Delegate<void, uint32_t> Reset;
    std::string TypeFilter;
};

// Contains a vector of properties of current frame
// These properties are only guaranteed to be valid the tick they were instantiated
struct PropertyContainer
{
    std::vector<Property> properties;

    // FIXME: This function must be called after container is not needed anymore to avoid memory leaks
    void Cleanup()
    {
        for (const Property &property : properties)
        {
            if (property.type == PropertyType::Object)
            {
                PropertyData_Object *deletePtr = (PropertyData_Object *)property.value;
                delete deletePtr;
            }
        }
    }

    PropertyContainer &PushPropertyInt(const std::string_view &name, int32_t *integer)
    {
        properties.push_back(Property(PropertyType::Integer, name, integer));
        return *this;
    }

    PropertyContainer &PushPropertyFloat(const std::string_view &name, float *fl)
    {
        properties.push_back(Property(PropertyType::Float, name, fl));
        return *this;
    }

    PropertyContainer &PushPropertyBoolean(const std::string_view &name, bool *boolean)
    {
        properties.push_back(Property(PropertyType::Boolean, name, boolean));
        return *this;
    }

    PropertyContainer &PushPropertyText(const std::string_view &name, Text *text)
    {
        properties.push_back(Property(PropertyType::Text, name, text));
        return *this;
    }

    template <ObjectClass _ObjectT>
    PropertyContainer &PushPropertyObject(const std::string_view &name, ObjectRef<_ObjectT> &ref)
    {
        properties.emplace_back(PropertyType::Object, name, 
                                new PropertyData_Object(Delegate<uint32_t>::CreateMemberConst(&ref, &ObjectRef<_ObjectT>::GetID), 
                                                        Delegate<void, uint32_t>::CreateMember(&ref, &ObjectRef<_ObjectT>::Reset),
                                                        _ObjectT::GetStaticType()));
        return *this;
    }

    PropertyContainer &PushPropertyString(const std::string_view &name, std::string *string)
    {
        properties.push_back(Property(PropertyType::String, name, string));
        return *this;
    }

    PropertyContainer &PushPropertyMethod(const std::string_view &name, Delegate<void> *method)
    {
        properties.push_back(Property(PropertyType::Method, name, method));
        return *this;
    }
};

class IPropertyProvider
{
public:
    virtual PropertyContainer GetEditorReflectedProperties() { return {}; }
};