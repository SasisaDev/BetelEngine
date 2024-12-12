#pragma once
#include <cstdint>
#include <unordered_map>
#include <string>

#include <i18n/Text.h>

class Artifact;

enum class ArtifactType : uint8_t
{
    Bool,
    Int32,
    UInt32,
    String,
    Artifact,
    Text,
    Byte,
    Custom,
};

class ArtifactElement
{
public:
    ArtifactType type;

    ArtifactElement(ArtifactType nType) : type(nType) {}
};

#define DefineArtifactElement(CLASSNAME, TYPE, ENUM) \
    class CLASSNAME : public ArtifactElement \
    { \
    public: \
        TYPE data; \
        CLASSNAME(TYPE nData) : ArtifactElement(ENUM), data(nData) {} \
    }

DefineArtifactElement(StringArtifactElement, std::string, ArtifactType::String);
DefineArtifactElement(BoolArtifactElement, bool, ArtifactType::Bool);
DefineArtifactElement(ByteArtifactElement, unsigned char, ArtifactType::Byte);
DefineArtifactElement(Int32ArtifactElement, int32_t, ArtifactType::Int32);
DefineArtifactElement(UInt32ArtifactElement, uint32_t, ArtifactType::UInt32);
DefineArtifactElement(NestedArtifactElement, Artifact*, ArtifactType::Artifact);

#undef DefineArtifactElement

class Artifact
{
protected:
    std::unordered_map<std::string, ArtifactElement*> dataBank;
public:
    Artifact operator+(const Artifact& otherArtifact);

    void AddBool(std::string key, bool value)                   {dataBank.insert_or_assign(key, new BoolArtifactElement(value));}
    void AddByte(std::string key, unsigned char value)          {dataBank.insert_or_assign(key, new ByteArtifactElement(value));}
    void AddInt32(std::string key, int32_t value)               {dataBank.insert_or_assign(key, new Int32ArtifactElement(value));}
    void AddUInt32(std::string key, uint32_t value)             {dataBank.insert_or_assign(key, new UInt32ArtifactElement(value));}
    void AddString(std::string key, std::string value)          {dataBank.insert_or_assign(key, new StringArtifactElement(value));}
    void AddArtifact(std::string key, Artifact* value)          {dataBank.insert_or_assign(key, new NestedArtifactElement(value));}
    void AddCustom(std::string key, size_t size, void* data);
    void AddText(std::string key, Text value);

    bool Has(std::string key) const {return dataBank.contains(key);}

    std::string GetStringOrDefault(std::string key, std::string def = "")   {return (Has(key) && dataBank[key]->type == ArtifactType::String) ? ((StringArtifactElement*)dataBank[key])->data : def;}
    bool GetBoolOrDefault(std::string key, bool def = false)                {return (Has(key) && dataBank[key]->type == ArtifactType::Bool) ? ((BoolArtifactElement*)dataBank[key])->data : def;}
    bool GetByteOrDefault(std::string key, unsigned char def = '\0')        {return (Has(key) && dataBank[key]->type == ArtifactType::Byte) ? ((ByteArtifactElement*)dataBank[key])->data : def;}
    bool GetInt32OrDefault(std::string key, int32_t def = 0)                {return (Has(key) && dataBank[key]->type == ArtifactType::Int32) ? ((Int32ArtifactElement*)dataBank[key])->data : def;}
    bool GetUInt32OrDefault(std::string key, uint32_t def = 0)              {return (Has(key) && dataBank[key]->type == ArtifactType::UInt32) ? ((UInt32ArtifactElement*)dataBank[key])->data : def;}
};