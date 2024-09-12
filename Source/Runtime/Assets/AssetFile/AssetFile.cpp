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
        file = IPlatform::Get()->OpenLocalFile(path, FILE_ACCESS_FLAG_WRITE | FILE_ACCESS_FLAG_BINARY);
    }


}
#endif

void AssetFile::ReadFromDevice(IFile* _file)
{
    IFile* file = _file;
    if(file == nullptr) {
        file = IPlatform::Get()->OpenLocalFile(path, FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY);
    }

    if(file == nullptr || !file->IsOpen()) {
        LOGF(Error, LogAsset, "Failed reading asset file \"%s\". Doesn't exist.", path.GetPath().c_str());
        return;
    }

    header.pSignature[0] = *file->Fetch(1);
    header.pSignature[1] = *file->Fetch(1);
    header.pSignature[2] = *file->Fetch(1);

    if(header.pSignature[0] != 'B' || header.pSignature[0] != 'A' || header.pSignature[0] != 'F') {
        LOGF(Error, LogAsset, "Failed reading asset file \"%s\". Not an asset file.", path.GetPath().c_str());
        return;
    }
}