#pragma once

#include <Object/Object.h>
#include <memory>
#include <RenderV/Objects/Images/SamplerTexture.h>

class ObjBrushSprite : public Object
{
    std::shared_ptr<ISamplerTexture> texture;
public:

    inline char* GetTextureData() const {return static_cast<char*>(texture->GetBuffer()->Read());}
    inline uint32_t GetWidth() const {return texture->GetWidth();}
    inline uint32_t GetHeight() const {return texture->GetHeight();}

    // TODO: Loading texture on serialization load event

};

class ObjBrushSpriteType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new ObjBrushSprite; }
    virtual std::string_view DisplayName() override {return "Brush Sprite";}
};