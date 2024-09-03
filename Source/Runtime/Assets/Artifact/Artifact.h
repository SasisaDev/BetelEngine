#pragma once
#include <cstdint>
#include <unordered_map>

enum class ArtifactType : uint8_t
{
    Bool,
    Int32,
    UInt32,
    String,
    Artifact,
    Text,
    Custom,
};

class ArtifactElement
{
    ArtifactType type;

    ArtifactElement(ArtifactType nType) : type(nType) {}
};

class Artifact
{
protected:
    std::unordered_map<std::string, ArtifactElement*> dataBank;
public:
    Artifact operator+(const Artifact& otherArtifact);

    void AddArtifact(std::string key, const Artifact& value);

    void AddBool(std::string key, bool value);
    void AddInt32(std::string key, int32_t value);
    void AddUInt32(std::string key, uint32_t value);
    void AddString(std::string key, std::string value);
    void AddCustom(std::string key, size_t size, void* data);
    // TODO: Create Localized Text
    //void AddText(std::string key, bool value);
};