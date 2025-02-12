#pragma once

#include <Object/Object.h>
#include <memory>
#include <RenderV/Objects/Images/SamplerTexture.h>

class ObjSprite : public Object
{
    std::shared_ptr<ISamplerTexture> texture;
public:

    // TODO: Rewrite to create dedicated buffer and unmap
    inline char* GetTextureData() const {return static_cast<char*>(texture->GetBuffer()->Map());}
    inline uint32_t GetWidth() const {return texture->GetWidth();}
    inline uint32_t GetHeight() const {return texture->GetHeight();}

    // TODO: Loading texture on serialization load event

};

class ObjSpriteType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new ObjSprite; }
    virtual bool CanSpawnIntoWorld() override {return true;}
    virtual Entity* CreateWorldEntity() override;
    virtual std::string_view DisplayName() override {return "Sprite";}
};