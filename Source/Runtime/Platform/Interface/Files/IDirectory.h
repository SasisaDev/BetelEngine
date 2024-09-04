#pragma once

#include <string>
#include <vector>
#include <cstdint>

enum EDirectoryFlags : uint8_t
{
	DIRECTORY_FLAG_RECURSIVE = (1 << 0),
    DIRECTORY_FLAG_CREATE = (1 << 1),
};

class IDirectory
{
    struct Entry {
        // TODO: Create path class
        bool isDirectory : 1;
        bool exists: 1; 
        std::string path;
        std::vector<Entry> nodes;
    };

    Entry entry;
    std::vector<IDirectory*> nestedDirectories;

    virtual std::vector<IDirectory::Entry> ParseNested(std::string path, bool recursive);
public:
    IDirectory(std::string path, uint8_t flags);
    IDirectory(const Entry& defaultEntry);

    virtual inline const Entry& GetEntry() const {return entry;}
    virtual inline const std::string& GetPath() const {return entry.path;}
    virtual inline bool IsDirectory() const {return entry.isDirectory;}
    virtual inline bool Exists() const {return entry.exists;}

    virtual const std::vector<IDirectory*>& GetChildren(); 
    virtual const std::vector<IDirectory*>& FetchChildren();
};