#include "AssetFile.h"
#include <Platform/Platform.h>
#include <Log/Logger.h>
#include <sstream>

AssetFile& AssetFile::operator<<(const Artifact& input)
{
    artifact = input;
    return *this;
}

AssetFile& AssetFile::operator>>(Artifact& output)
{
    for(uint16_t i = 0; i < body.uVariableCount; ++i) {
        switch((EAssetVarType)body.pVariables[i].uVariableType) {
            case EAssetVarType::String:
                artifact.AddString(body.pVariables[i].pVariableName, reinterpret_cast<const char*>(body.pVariables[i].pVariableData));
                break;
            case EAssetVarType::Bool:
                artifact.AddBool(body.pVariables[i].pVariableName, *reinterpret_cast<bool*>(body.pVariables[i].pVariableData));
                break;
        }
    }

    output = artifact;
    return *this;
}

void AssetFile::WriteToDevice()
{
    std::unique_ptr<IFile> file = IPlatform::Get()->OpenLocalFile(path + ".asset", FILE_ACCESS_FLAG_WRITE | FILE_ACCESS_FLAG_BINARY);

    if(file == nullptr || !file->IsOpen()) {
        LOGF(Error, LogAsset, "Failed writing asset file \"%s\".", path.c_str());
        return;
    }

    std::stringstream buf;

    buf << header.pSignature << *reinterpret_cast<char*>(header.uMagic) << *reinterpret_cast<char*>(header.uVersion) << *reinterpret_cast<char*>(header.uFactoryVersion);

    buf << *reinterpret_cast<char*>(header.uTypeNameLength) << header.pTypeName;

    file->Write(buf.str());
}

void AssetFile::ReadFromDevice()
{
    std::unique_ptr<IFile> file = IPlatform::Get()->OpenLocalFile(path + ".asset", FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY);

    if(file == nullptr || !file->IsOpen()) {
        LOGF(Error, LogAsset, "Failed reading asset file \"%s\". Doesn't exist.", path.c_str());
        return;
    }

    char* buffer;
#   define CHECKREAD(count) buffer = file->Fetch(count); if(buffer == nullptr) {LOGF(Error, LogAsset, "Failed reading asset file \"%s\". EOF was not expected", path.c_str()); return;}

    header.pSignature[0] = *file->Fetch(1);
    header.pSignature[1] = *file->Fetch(1);
    header.pSignature[2] = *file->Fetch(1);

    if(header.pSignature[0] != 'B' || header.pSignature[1] != 'A' || header.pSignature[2] != 'F') {
        LOGF(Error, LogAsset, "Failed reading asset file \"%s\". Not an asset file.", path.c_str());
        return;
    }

    header.uMagic = *reinterpret_cast<uint16_t*>(file->Fetch(2));
    if(header.uMagic != 0xBE7E) {
        LOGF(Error, LogAsset, "Failed reading asset file \"%s\". Magic is wrong.", path.c_str());
        return;
    }

    header.uVersion = *reinterpret_cast<uint16_t*>(file->Fetch(2));
    if(header.uVersion > BETEL_ASSET_FILE_VERSION) {
        LOGF(Error, LogAsset, "Failed reading asset file \"%s\". It's created using newer editor. File version is %u, while engine version is %u", path.c_str(), header.uVersion, BETEL_ASSET_FILE_VERSION);
        return;
    }

    // Version of factory of asset type, can't check
    header.uFactoryVersion = *reinterpret_cast<uint16_t*>(file->Fetch(2));

    // Get Asset Typename
    CHECKREAD(2);
    header.uTypeNameLength = *reinterpret_cast<uint16_t*>(buffer);
    header.pTypeName = file->Fetch(header.uTypeNameLength);

    assetTypeName = std::string(header.pTypeName);

    // Body start
    CHECKREAD(2);
    body.uVariableCount = *reinterpret_cast<uint16_t*>(buffer);
    body.pVariables = new AssetFileVariable[body.uVariableCount];
    for(int varIdx = 0; varIdx < body.uVariableCount; ++varIdx) {
        AssetFileVariable variable;
        variable.uVariableDataLength = 0;
        bool ignored = false;

        CHECKREAD(2);
        variable.uVariableType = *reinterpret_cast<uint16_t*>(buffer);
        if(variable.uVariableType >= (uint16_t) EAssetVarType::LAST) {
            LOGF(Error, LogAsset, "Error while reading asset file \"%s\". Unknown variable type %u. Will be ignored", path.c_str(), variable.uVariableType);
            ignored = true;
        }
        
        CHECKREAD(2);
        variable.uVariableNameLength = *reinterpret_cast<uint16_t*>(buffer);
        if(variable.uVariableNameLength < 2) {
            ignored = true;
            LOGF(Error, LogAsset, "Error while reading asset file \"%s\". Impossible name. Will be ignored.", path.c_str(), variable.uVariableType);
        }

        CHECKREAD(variable.uVariableNameLength);
        variable.pVariableName = buffer;

        // No need to fetch data length, if it's already known from type
        switch((EAssetVarType)variable.uVariableType)
        {
            case EAssetVarType::Int:
                variable.uVariableDataLength = 4;
                break;
            case EAssetVarType::Byte:
                variable.uVariableDataLength = 1;
                break;
            case EAssetVarType::Bool:
                variable.uVariableDataLength = 1;
                break;
            default:
                // If data length can't be deduced from type, next 4 bytes will specify it
                CHECKREAD(4);
                variable.uVariableDataLength = *reinterpret_cast<uint32_t*>(buffer);
                break;
        }

        CHECKREAD(variable.uVariableDataLength);
        variable.pVariableData = buffer;

        if(!ignored) {
            body.pVariables[varIdx] = variable;
        }
    }

    LOGF(Log, LogAsset, "Loaded asset file \"%s\".", path.c_str());

#   undef CHECKREAD
}