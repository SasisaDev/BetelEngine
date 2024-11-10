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

void IFile::Seek(size_t offset, uint8_t seekOrigin)
{
    file.seekg(offset, ((seekOrigin == FILE_SEEK_FLAG_BEG) ? std::ios::beg : 
                       ((seekOrigin == FILE_SEEK_FLAG_EOF) ? std::ios::end :
                         std::ios::cur)) );
}

char* IFile::Peek(size_t count)
{
    char* buffer = new char[count];
    
    size_t oldCursor = file.tellg();
    file.read(buffer, count);

    file.seekg(oldCursor, std::ios::beg);
    return buffer;
}

char* IFile::Fetch(size_t count)
{
    if(file.eof() || file.peek() == EOF) {
        return nullptr;
    }

    char* buffer = new char[count];
    file.read(buffer, count);
    return buffer;
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