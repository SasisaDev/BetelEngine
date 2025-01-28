#pragma once

#include <Object/Object.h>
#include <memory>
#include <GameFramework/Assets/Texture/Texture.h>
#include <RenderV/Objects/Images/SamplerTexture.h>
#include <Object/ObjectRef.h>

class ObjTexture : public Object
{
    std::unique_ptr<ISamplerTexture> texture;
    uint32_t sizeX = 0, sizeY = 0;
    std::string path;
public:

    void SetPath(const std::string& newPath) {path = newPath;}

    inline ISamplerTexture* GetTexture() const {return texture.get();}
    inline uint32_t GetWidth() const {return sizeX;}
    inline uint32_t GetHeight() const {return sizeY;}
    inline const std::string& GetPath() const {return path;}

    // Should be called by itself after Serialization call complete
    void LoadTexture();

    virtual void Serialize(FieldContainer& container) override;
};

class ObjTextureType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new ObjTexture; }
    virtual std::string_view DisplayName() override {return "Texture";}
};