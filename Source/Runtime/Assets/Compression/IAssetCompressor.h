#pragma once

#include <vector>

class IAssetCompressor {
public:
    virtual std::vector<char> Compress(std::vector<char> data) {return data;}
    virtual std::vector<char> Decompress(std::vector<char> data, int originalSize) {return data;}
};