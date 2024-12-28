#pragma once

#include <Log/Logger.h>

#include "ICompressor.h"

#include <lz4/lz4.h>
#include <lz4/lz4hc.h>

// TODO: LZ4 Compressor
class LZ4Compressor : public ICompressor
{
public:
    virtual std::vector<char> Compress(std::vector<char> data) override 
    {
        std::vector<char> compressed;
        int boundSize = LZ4_compressBound(data.size());
        compressed.resize(boundSize);
        int compressedSize = LZ4_compress_HC(data.data(), compressed.data(), data.size(), boundSize, 9);
        compressed.resize(compressedSize);
        return compressed;
    }
    
    virtual std::vector<char> Decompress(std::vector<char> data, int originalSize) override 
    {
        std::vector<char> decompressed;
        decompressed.resize(originalSize);
        if(LZ4_decompress_safe(data.data(), decompressed.data(), data.size(), originalSize) < 0) {
            LOG(Error, LogLZ4, "Failed asset decompression");
            return data;
        }
        return decompressed;
    }
};