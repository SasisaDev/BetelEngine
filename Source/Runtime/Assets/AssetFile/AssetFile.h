#pragma once

#include <cstdint>

struct AssetFileHeader
{
    unsigned char signature[3];
    uint8_t version;

};

