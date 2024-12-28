#pragma once

#include <Assets/Asset.h>

class Texture2D : public Asset
{
    char* textureData;
    uint32_t sizeX = 0, sizeY = 0;
public:

    inline char* GetTextureData() const {return textureData;}
    inline uint32_t GetWidth() const {return sizeX;}
    inline uint32_t GetHeight() const {return sizeY;}

};