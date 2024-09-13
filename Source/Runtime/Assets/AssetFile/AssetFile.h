#pragma once

#include <cstdint>
#include <string>
#include <Platform/Platform.h>

#include "../Artifact/Artifact.h"

#define BETEL_ASSET_FILE_VERSION 1

enum class EAssetVarType : uint16_t
{
    Int = 0,
    Byte = 1,
    String = 2,
    Text = 3,
    Struct = 4,
    Custom = 5,
    // Used to check top bound when parsing 
    LAST
};

struct AssetFileHeader
{
    // Betel Asset File
    unsigned char pSignature[3] = {'B', 'A', 'F'};
    uint16_t uMagic = 0xBE7E;
    uint16_t uVersion = BETEL_ASSET_FILE_VERSION;
    uint16_t uFactoryVersion;
    uint16_t uTypeNameLength;
    char* pTypeName;
};

struct AssetFileVariable
{
    uint16_t uVariableType;
    uint16_t uVariableNameLength;
    char* pVariableName;
    uint32_t uVariableDataLength;
    void* pVariableData;
};

struct AssetFileBody
{
    uint16_t uVariableCount;
    AssetFileVariable* pVariables;
};

class AssetFile
{
    AssetFileHeader header;
    AssetFileBody body;

    Artifact artifact;
    std::string assetTypeName;
    std::string path;
public:
    AssetFile() {}
    AssetFile(std::string filePath) : path(filePath) {}

    inline std::string GetAssetTypeName() const {return assetTypeName;}

    AssetFile& operator<<(const Artifact& input);
    AssetFile& operator>>(Artifact& output);

#ifdef EDITOR
    void WriteToDevice(IFile* file = nullptr);
#endif
    void ReadFromDevice(IFile* file = nullptr);
};