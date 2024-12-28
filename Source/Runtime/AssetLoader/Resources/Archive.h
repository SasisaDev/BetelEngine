#pragma once

#include <unordered_map>
#include <string>

class ArchiveFile {
    std::unordered_map<std::string, uint32_t> Table;
};