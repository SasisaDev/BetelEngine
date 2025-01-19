#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>

enum class ObjectFieldType : uint8_t
{
    // int32_t
    Int,
    // uint32_t
    UInt,
    // float
    Float,
    // double
    Double,
    // {uint32_t, const char*}
    String,
    // {uint32_t, const char*}
    Text,
    // uint32_t
    Object,
    /* 
     * {
     *     ObjectFieldType type;
     *     uint32_t count;
     *     void* data;
     * }
     */
    Array,
    /* 
     * {
     *     ObjectFieldType keyType;
     *     ObjectFieldType valueType;
     *     uint32_t count;
     *     {void* key, void* value}[count];
     * }
     */
    Map,
    // {uint32_t, void*}
    Custom,
};

struct ObjectField
{
    ObjectFieldType Type;
    ObjectField(ObjectFieldType type) : Type(type) {}

    virtual inline std::vector<char> Serialize() {}
    virtual void Deserialize(uint32_t size, char* buffer){}
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

    virtual inline std::vector<char> Serialize() override {
        std::vector<char> buffer(4);
        buffer[0] = (Data >> 24) & 0xFF;
        buffer[1] = (Data >> 16) & 0xFF;
        buffer[2] = (Data >> 8) & 0xFF;
        buffer[3] = Data & 0xFF;
        return buffer;
    }

    virtual void Deserialize(uint32_t size, char* buffer) override {
        Data =  (int32_t)buffer[3] << 24 |
                (int32_t)buffer[2] << 16 |
                (int32_t)buffer[1] << 8  |
                (int32_t)buffer[0];
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

    virtual inline std::vector<char> Serialize() override {
        std::vector<char> buffer;
        
        buffer.resize(sizeof(uint32_t) + Data.size());
        buffer[0] = (Data.size() >> 24) & 0xFF;
        buffer[1] = (Data.size() >> 16) & 0xFF;
        buffer[2] = (Data.size() >> 8) & 0xFF;
        buffer[3] = Data.size() & 0xFF;

        buffer.insert(buffer.begin() + 4, Data.begin(), Data.end());

        return buffer;
    }

    virtual void Deserialize(uint32_t size, char* buffer) override {
        Data = std::string(buffer);
    }
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
    
};

#undef GetAndSet