#include "IFile.h"

IFile::IFile(std::string path, uint8_t accessFlags)
{
    std::ios_base::openmode mode;

    if((accessFlags & FILE_ACCESS_FLAG_READ) == FILE_ACCESS_FLAG_READ)
    {
        mode = std::ios::in;
    } 
    else 
    {
        mode = std::ios::out;
    }

    if((accessFlags & FILE_ACCESS_FLAG_BINARY) == FILE_ACCESS_FLAG_BINARY)
    {
        mode = mode | std::ios::binary;
    }
    if((accessFlags & FILE_ACCESS_FLAG_ATE) == FILE_ACCESS_FLAG_ATE)
    {
        mode = mode | std::ios::ate;
    }

    file.open(path, mode);
}

IFile::~IFile()
{
    file.close();
}

const char* IFile::Fetch(size_t count)
{
    return nullptr;
}

std::string IFile::FetchAll()
{
    std::string result;

    if (file.is_open()) {
        std::streampos size = file.tellg();
        result.resize(size);
        file.seekg (0, std::ios::beg);
        file.read (result.data(), size);
    }

    return result;
}

std::vector<char> IFile::FetchAllBinary()
{
    std::vector<char> result;

    if (file.is_open()) {
        std::streampos size = file.tellg();
        result.resize(size);
        file.seekg (0, std::ios::beg);
        file.read (result.data(), size);
    }

    return result;
}