#pragma once

#include <string>
#include <vector>
#include <cstdint>

enum EFileAccessFlags : uint8_t
{
	DIRECTORY_FLAG_RECURSIVE = (1 << 0),
    DIRECTORY_FLAG_CREATE = (1 << 1),
};

class IDirectory
{
    struct Entry {
        // TODO: Create path class
        std::string path;
        std::vector<Entry> directories;
    };

    Entry entry;
public:
    IDirectory(std::string path, uint8_t flags);
};