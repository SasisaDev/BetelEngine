#pragma once

#include <unordered_map>

struct BlameObjectTableEntry {
    uint32_t offset, size;
};

/*
 * Blame Master Files are the main engine files, containing the "Initial Commit" of game objects
 * It doesn't have any change records, just pure full data for main game objects set
 * 
 * Contains Lookup-Table ID-Offset-Size (uint32_t each)
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
public:

};