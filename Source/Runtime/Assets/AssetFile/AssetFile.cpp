#include "AssetFile.h"
#include <Platform/Platform.h>

AssetFile& AssetFile::operator<<(const Artifact& input)
{
    artifact = input;
    return *this;
}

AssetFile& AssetFile::operator>>(Artifact& input)
{
    input = artifact;
    return *this;
}

void AssetFile::WriteToDevice(IFile* file)
{
    if(file == nullptr) {
        file = IPlatform::Get()->OpenLocalFile(path, FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY);
    }


}

void AssetFile::ReadFromDevice(IFile* file)
{

}