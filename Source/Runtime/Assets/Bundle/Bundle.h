#pragma once

#include <Platform/Platform.h>

#include "../AssetFile/AssetFile.h"
#include "../Compression/IAssetCompressor.h"

struct BundleMetaTableEntry {
    uint16_t uPathLength;
    char* pPath;
    // Size of AssetFile before compression (decompressed)
    uint32_t uOriginalSize;
    // Current size of AssetFile after compression (compressed)
    uint32_t uCurrentSize;
    // Current position of AssetFile in file, counting from file's first byte
    uint32_t uOffset;
};

class BundleFile
{
    IFile* file;
    std::vector<BundleMetaTableEntry> metatable;
public:
    AssetFile* ReadAssetFile(std::string path, IAssetCompressor* compressor = nullptr) {return nullptr;}
    AssetFile* WriteAssetFile(std::string path, IAssetCompressor* compressor = nullptr) {return nullptr;}
};