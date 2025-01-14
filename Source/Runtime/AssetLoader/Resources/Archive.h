#pragma once

#include <unordered_map>
#include <string>

#define ARCHIVE_FILE_EXT "arch"
#define ARCHIVE_FILE_MAGIC 0xBE7E1ACH1F
#define ARCHIVE_FILE_VERSION 0

class ArchiveFile {
    std::unordered_map<std::string, uint32_t> Table;
};