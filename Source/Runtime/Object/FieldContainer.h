#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>

enum class ObjectFieldType : uint8_t
{
    // int32_t
    Int = 0x00,
    // uint32_t
    UInt = 0x01,
    // float
    Float = 0x02,
    // double
    Double = 0x03,
    // const char*
    String = 0x04,
    // const char*
    Text = 0x05,
    // uint32_t
    Object = 0x06,
    /* 
     * {
     *     ObjectFieldType type;
     *     uint32_t count;
     *     char* data;
     * }
     */
    Array = 0x07,
    /* 
     * {
     *     ObjectFieldType keyType;
     *     ObjectFieldType valueType;
     *     uint32_t count;
     *     {char* key, char* value}[count];
     * }
     */
    Map = 0x08,
    // char*
    Custom = 0x09,
};

struct ObjectField
{
    ObjectFieldType Type;
    ObjectField(ObjectFieldType type) : Type(type) {}
};

struct ObjectFieldInt : public ObjectField
{
    ObjectFieldInt() : ObjectField(ObjectFieldType::Int) {}
    int32_t Data;

    void Store(int32_t newData) {
        Data = newData;
    }

    int32_t Load() {
        return Data;
    }
};

struct ObjectFieldUInt : public ObjectField
{
    ObjectFieldUInt() : ObjectField(ObjectFieldType::UInt) {}
    uint32_t Data;

    void Store(uint32_t newData) {
        Data = newData;
    }

    uint32_t Load() {
        return Data;
    }
};

struct ObjectFieldFloat : public ObjectField
{
    ObjectFieldFloat() : ObjectField(ObjectFieldType::Float) {}
    float Data;

    void Store(float newData) {
        Data = newData;
    }

    float Load() {
        return Data;
    }
};

struct ObjectFieldDouble : public ObjectField
{
    ObjectFieldDouble() : ObjectField(ObjectFieldType::Double) {}
    double Data;

    void Store(double newData) {
        Data = newData;
    }

    double Load() {
        return Data;
    }
};

struct ObjectFieldString : public ObjectField
{
    ObjectFieldString() : ObjectField(ObjectFieldType::String) {}
    std::string Data;

    void Store(const std::string& newData) {
        Data = newData;
    }

    const std::string& Load() {
        return Data;
    }
};

struct ObjectFieldText : public ObjectFieldString
{
    ObjectFieldText() {Type = ObjectFieldType::Text;}
};

struct ObjectFieldObject : public ObjectFieldUInt
{
    ObjectFieldObject() {Type = ObjectFieldType::Object;}
};

#define GetAndSet(ObjectType, CType, Def) \
void Set##ObjectType (std::string Name, CType Value) {\
    if(Fields.contains(Name)) {\
        if(Fields[Name]->Type == ObjectFieldType::##ObjectType){ \
            ((ObjectField##ObjectType *)(Fields[Name]))->Store(Value);\
        } else {\
            delete Fields[Name]; \
            Fields[Name] = new ObjectField##ObjectType();\
        } \
    } else{\
        Fields[Name] = new ObjectField##ObjectType();\
    }\
}\
\
CType Get##ObjectType (std::string Name, CType Default = Def) {\
    if(!Fields.contains(Name) || Fields[Name]->Type != ObjectFieldType::##ObjectType) {\
        return Default;\
    }\
    return ((ObjectField##ObjectType *)(Fields[Name]))->Load();\
}

class FieldContainer
{
    std::unordered_map<std::string, ObjectField*> Fields;
public:
    FieldContainer(){}
    FieldContainer(size_t Size) {
        Fields.reserve(Size);
    }

    GetAndSet(Int, int32_t, 0);
    GetAndSet(UInt, uint32_t, 0);
    GetAndSet(Float, float, 0);
    GetAndSet(Double, double, 0);
    GetAndSet(String, const std::string&, {});
    GetAndSet(Text, const std::string&, {});
    GetAndSet(Object, uint32_t, 0);
    
};

#undef GetAndSet