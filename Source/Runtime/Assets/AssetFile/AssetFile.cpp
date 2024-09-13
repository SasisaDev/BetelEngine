#include "AssetFile.h"
#include <Platform/Platform.h>
#include <Log/Logger.h>

AssetFile& AssetFile::operator<<(const Artifact& input)
{
    artifact = input;
    return *this;
}

AssetFile& AssetFile::operator>>(Artifact& output)
{
    output = artifact;
    return *this;
}

#ifdef EDITOR
void AssetFile::WriteToDevice(IFile* file)
{
    if(file == nullptr) {
        file = IPlatform::Get()->OpenLocalFile(path + ".asset", FILE_ACCESS_FLAG_WRITE | FILE_ACCESS_FLAG_BINARY);
    }


}
#endif

void AssetFile::ReadFromDevice(IFile* _file)
{
    IFile* file = _file;
    if(file == nullptr) {
        file = IPlatform::Get()->OpenLocalFile(path + ".asset", FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY);
    }

    if(file == nullptr || !file->IsOpen()) {
        LOGF(Error, LogAsset, "Failed reading asset file \"%s\". Doesn't exist.", path.c_str());
        return;
    }

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
    header.uTypeNameLength = *reinterpret_cast<uint16_t*>(file->Fetch(2));
    header.pTypeName = file->Fetch(header.uTypeNameLength);

    LOGF(Log, LogAsset, "Loaded asset file \"%s\".", path.c_str());
}