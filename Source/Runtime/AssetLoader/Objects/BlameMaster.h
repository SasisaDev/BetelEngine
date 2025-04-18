#pragma once

#include <unordered_map>
#include <cstdint>
#include <string>
#include <memory>
#include <Platform/Interface/Files/IFile.h>

#define BLAME_MASTER_FILE_EXT "bem"
#define BLAME_MASTER_FILE_MAGIC 0xBE7E1115
#define BLAME_MASTER_FILE_VERSION 0

/*
 * Blame Master File Conventions
 *
 * TODO: All string values must consist of 16bit length and non-null-terminated string preceeding it 
 */

struct BlameMasterFileHeader {
    char pSignature[3] = {'B','M','F'};
    uint32_t uMagic = BLAME_MASTER_FILE_MAGIC;
    uint16_t uVersion = BLAME_MASTER_FILE_VERSION;
};

struct BlameObjectTableEntry {
    uint32_t ID = 0, offset = 0;
};

struct BlameMasterFileTable {
    uint32_t uObjectCount = 0;
    BlameObjectTableEntry* pObjects = nullptr;

    /*~BlameMasterFileTable() {
        delete[] pObjects;
    }*/
};

struct BlameMasterFileObjectField {
    uint8_t uFieldNameLength = 0;
    char* pFieldName = nullptr;
    uint8_t uType = 0;
    uint32_t uDataSize = 0;
    void* pData = nullptr;

    /*~BlameMasterFileObjectField() {
        delete[] pFieldName;
        delete[] pData;
    }*/
};

struct BlameMasterFileObject {
    uint8_t uClassNameLength = 0;
    char* pClassName = nullptr;
    uint16_t uNameLength = 0;
    char* pName = nullptr;
    uint32_t uParent = 0;
    uint16_t uFieldsCount = 0;
    BlameMasterFileObjectField* pFields = nullptr;

    /*~BlameMasterFileObject() {
        delete[] pClassName;
        delete[] pName;
        delete[] pFields;
    }*/
};

// Used to store a pointer to an Object, that will then be copied into memory
struct BlameMasterFileObjectContainer {
    uint32_t uObjectSize = 0;
    BlameMasterFileObject object;
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
    friend class AssetLoader;
    /* 
     * Blame Files are never loaded in full. This flag only indicates that it's data is open for access
     * This variable being false means, that file exists in the File System, but should not be interacted with
    */ 
    bool bIsMounted = false;

    std::unordered_map<uint32_t, uint32_t> Table;
    std::string FilePath;
    std::string FileName;
    std::unique_ptr<IFile> FileHandle;
protected:
    BlameMasterFileHeader FileHeader;
    BlameMasterFileTable FileTable;
public:

    std::string& GetName() {return FileName;}
    void SetName(const std::string& Name) {FileName = Name;}

    std::string& GetPath() {return FilePath;}
    void SetPath(const std::string& Path) {FilePath = Path;}

    bool IsMounted() {return bIsMounted;}
    void SetMounted(bool isMounted) {bIsMounted = isMounted;}

    const BlameMasterFileHeader& GetHeader() {return FileHeader;}
    const BlameMasterFileTable& GetFileTable() {return FileTable;}
};