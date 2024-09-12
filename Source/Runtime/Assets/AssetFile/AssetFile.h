#pragma once

#include <cstdint>
#include <string>
#include <Platform/Platform.h>

#include "../Artifact/Artifact.h"

enum class EAssetVarType : uint16_t
{
    Int = 0,
    Byte = 1,
    String = 2,
    Text = 3,
    Struct = 4,
    Custom = 5
};

struct AssetFileHeader
{
    // Betel Asset File
    unsigned char pSignature[3] = {'B', 'A', 'F'};
    uint16_t uMagic = 0xBE7E;
    uint16_t uVersion = 1;
    uint16_t uFactoryVersion;
    uint16_t uClassNameLength;
    char* pClassName;
};

struct AssetFileVariable
{
    EAssetVarType uVariableType;
    uint16_t uVariableNameLength;
    char* pVariableName;
    uint32_t uVariableDataLength;
    void* pVariableData;
};

struct AssetFileBody
{
    uint16_t u;
};

class AssetFile
{
    Artifact artifact;
    IPath path;
public:
    AssetFile() {}
    AssetFile(IPath filePath) : path(filePath) {}

    AssetFile& operator<<(const Artifact& input);
    AssetFile& operator>>(Artifact& input);

#ifdef EDITOR
    void WriteToDevice(IFile* file = nullptr);
#endif
    void ReadFromDevice(IFile* file = nullptr);
};