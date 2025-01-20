#pragma once

#include <map>
#include <string>

#define ARCHIVE_FILE_EXT "arch"
#define ARCHIVE_FILE_MAGIC 0xBE7E1ACH1F
#define ARCHIVE_FILE_VERSION 0

struct ArchiveFileHeader {
    char pSignature[3] = {'B','M','F'};
    uint32_t uMagic = BLAME_MASTER_FILE_MAGIC;
    uint16_t uVersion = BLAME_MASTER_FILE_VERSION;
};

struct ArchiveResourceTableEntry {
    uint64_t hash = 0, offset = 0;
};

struct ArchiveFileTable {
    uint32_t uEntryCount = 0;
    BlameObjectTableEntry* pEntries = nullptr;
};

struct ArchiveFileResource {
    uint32_t uDataSize = 0;
    char* pData = nullptr;
};

class ArchiveFile {
    friend class AssetLoader;
    
    std::map<uint64_t, uint64_t> Table;
};

struct ArchiveUtility {
    static uint64_t Hash64(const std::string& str) {
        const int p = 53;
        const int m = 1e9 + 9;
        uint64_t hash_value = 0;
        uint64_t p_pow = 1;
        for (char c : str) {
            hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
            p_pow = (p_pow * p) % m;
        }
        return hash_value;
    }
};