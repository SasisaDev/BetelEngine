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

IPath IPlatform::GetContentPathAlias(std::string alias) 
{
    IPath mainAlias = PathAliases[alias] + std::string("Content/");
    if(alias == "Game") {
        return mainAlias;
    }

    // In case Editor & Project are in the same directory
    // This usually happens when project is already shipped, 
    // And the content is already packed inside the ./Content/Editor folder
    if(GetPathAlias("Game").GetPath() == GetPathAlias("Editor").GetPath()) {
        mainAlias = mainAlias + std::string("Editor/");
    }

    return mainAlias;
}

IFile* IPlatform::OpenContentFile(std::string path, uint8_t flags)
{
    std::string localPath = {};
    std::string domain = Internal_SeparateLocalPathDomain(path, &localPath);

    if(!PathAliases.contains(domain)) {
        return nullptr;
    }

    IPath realDirPath = GetContentPathAlias(domain) + localPath; 

    return OpenFile(realDirPath, flags);
}

IDirectory* IPlatform::OpenContentDirectory(std::string path, uint8_t flags)
{
    std::string localPath = {};
    std::string domain = Internal_SeparateLocalPathDomain(path, &localPath);

    if(!PathAliases.contains(domain)) {
        return nullptr;
    }

    IPath realDirPath = GetContentPathAlias(domain) + localPath; 

    return OpenDirectory(realDirPath, flags);
}