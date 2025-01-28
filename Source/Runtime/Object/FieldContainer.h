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
    ObjectFieldInt(int32_t dat) : ObjectField(ObjectFieldType::Int), Data(dat) {}
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
    ObjectFieldUInt(uint32_t dat) : ObjectField(ObjectFieldType::UInt), Data(dat) {}
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
    ObjectFieldFloat(float dat) : ObjectField(ObjectFieldType::Float), Data(dat) {}
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
    ObjectFieldDouble(double dat) : ObjectField(ObjectFieldType::Double), Data(dat) {}
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
    ObjectFieldString(const std::string & nDat) : ObjectField(ObjectFieldType::String), Data(nDat) {}
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
    ObjectFieldText(const std::string& dat) : ObjectFieldString(dat) {Type = ObjectFieldType::Text;}
};

struct ObjectFieldObject : public ObjectFieldUInt
{
    ObjectFieldObject(uint32_t id) : ObjectFieldUInt(id) {Type = ObjectFieldType::Object;}
};

#define GetAndSet(ObjectType, CType, Def) \
void Set##ObjectType (const std::string& Name, CType Value) {\
    if(Fields.contains(Name)) {\
        if(Fields[Name]->Type == ObjectFieldType::ObjectType){ \
            ((ObjectField##ObjectType *)(Fields[Name]))->Store(Value);\
        } else {\
            delete Fields[Name]; \
            Fields[Name] = new ObjectField##ObjectType( Value );\
        } \
    } else{\
        Fields[Name] = new ObjectField##ObjectType( Value );\
    }\
}\
\
CType Get##ObjectType (const std::string& Name, CType Default = Def) {\
    if(!Fields.contains(Name) || Fields[Name]->Type != ObjectFieldType::ObjectType) {\
        return Default;\
    }\
    return ((ObjectField##ObjectType *)(Fields[Name]))->Load();\
}

class FieldContainer
{
    std::unordered_map<std::string, ObjectField*> Fields;
    bool bSaving = false;
public:
    FieldContainer(){}
    FieldContainer(size_t Size) {
        Fields.reserve(Size);
    }

    inline bool IsSaving() const {return bSaving;}
    inline void SetSaving() {bSaving = true;}

    GetAndSet(Int, int32_t, 0);
    GetAndSet(UInt, uint32_t, 0);
    GetAndSet(Float, float, 0);
    GetAndSet(Double, double, 0);
    GetAndSet(String, const std::string&, {});
    GetAndSet(Text, const std::string&, {});
    GetAndSet(Object, uint32_t, 0);
    
};

#undef GetAndSet