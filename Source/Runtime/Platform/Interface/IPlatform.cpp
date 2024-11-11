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
    std::string localPath = {};
    std::string domain = Internal_SeparateLocalPathDomain(path, &localPath);

    if(!PathAliases.contains(domain)) {
        return nullptr;
    }

    IPath realFilePath = PathAliases[domain] + localPath; 

    return OpenFile(realFilePath, flags);
}

IDirectory* IPlatform::OpenLocalDirectory(std::string path, uint8_t flags)
{
    std::string localPath = {};
    std::string domain = Internal_SeparateLocalPathDomain(path, &localPath);

    if(!PathAliases.contains(domain)) {
        return nullptr;
    }

    IPath realDirPath = PathAliases[domain] + localPath; 

    return OpenDirectory(realDirPath, flags);
}