#pragma once

#include <Object/Object.h>
#include <memory>
#include <GameFramework/Assets/Texture/Texture.h>
#include <Object/ObjectRef.h>

class ObjTexture : public Object
{
    char* textureData;
    uint32_t sizeX = 0, sizeY = 0;
public:

    inline char* GetTextureData() const {return textureData;}
    inline uint32_t GetWidth() const {return sizeX;}
    inline uint32_t GetHeight() const {return sizeY;}

};

class ObjTextureType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new ObjTexture; }
};