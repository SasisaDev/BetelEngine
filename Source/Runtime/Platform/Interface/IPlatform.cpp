#include "IPlatform.h"

std::string IPlatform::Internal_SeparateLocalPathDomain(std::string in, std::string* purePath)
{
    if(in.size() <= 0) {
        return {};
    }

    std::string domain;

    size_t offset = 0;
    char curChar = in[0];

    while(offset < in.size())
    {
        curChar = in[offset++];

        // Stop parsing as soon as we find end of path token
        if(curChar == '/' || curChar == '\\') 
            break;

        domain.push_back(curChar);
    }

    if(purePath) {
        purePath->clear();
        purePath->insert(purePath->end(), in.begin()+offset, in.end());
    }

    return domain;
}

IFile* IPlatform::OpenLocalFile(std::string path, uint8_t flags) 
{
    IPath realFilePath = ExecutableLocalPath + path;

    return OpenFile(realFilePath, flags);
}

IDirectory* IPlatform::OpenLocalDirectory(std::string path, uint8_t flags)
{
    IPath realDirPath = ExecutableLocalPath + path;

    return OpenDirectory(realDirPath, flags);
}

IFile* IPlatform::OpenContentFile(std::string path, uint8_t flags)
{
    IPath realFilePath = ExecutableLocalPath + IPath("./Content/") + path; 

    return OpenFile(realFilePath, flags);
}

IDirectory* IPlatform::OpenContentDirectory(std::string path, uint8_t flags)
{
    IPath realDirPath = ExecutableLocalPath + IPath("./Content/") + path;

    return OpenDirectory(realDirPath, flags);
}