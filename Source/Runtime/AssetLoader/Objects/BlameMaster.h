#pragma once

#include <unordered_map>
#include <cstdint>
#include <string>

struct BlameMasterFileHeader {
    char pSignature[3] = {'B','M','F'};
    uint32_t uMagic = 0xBE7E1115AF;

};

struct BlameObjectTableEntry {
    uint32_t ID = 0, offset = 0;
};

struct BlameMasterFileTable {
    uint32_t uObjectCount = 0;
    BlameObjectTableEntry* pObjects = nullptr;
};

/*
 * 
 * TODO
 * 
*/
struct BlameMasterFileObject {
    uint32_t uSize = 0;
    uint32_t uClassNameLength = 0;
    char*   pClassName = nullptr;
    void* pData = nullptr;
};

/*
 * Blame Master Files are the main engine files, containing the "Initial Commit" of game objects
 * It doesn't have any change records, just pure full data for main game objects set
 * 
 * Contains Lookup-Table ID-Offset (uint32_t each)
 * First 4 bytes of Table specify number of Entries in the table.
 * 
 * Offset leads to the Data Blob, where Object is located. This part is appended at the end of file.
 * 
 * Structure of a file
 * ------------------
 * |     Header     |
 * ------------------
 * |     Table      |
 * ------------------
 * |   Data Blob    |
 * ------------------
*/
class BlameMasterFile
{
    /* 
     * Blame Files are never loaded in full. This flag only indicates that it's data is open for access
     * This variable being false means, that file exists in the File System, but should not be interacted with
    */ 
    bool IsMounted = false;

    std::unordered_map<uint32_t, BlameObjectTableEntry> Table;
    std::string FilePath;
    std::string FileName;
public:

};