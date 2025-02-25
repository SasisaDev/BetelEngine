#pragma once

#include <Object/Object.h>
#include <map>
#include <memory>
#include <GameFramework/Assets/Texture/Texture.h>
#include <Object/ObjectRef.h>
#include <Math/Vector.h>

class ObjAtlas : public Object
{
    ObjectRef<ObjTexture> texture;
    std::map<uint16_t, IVec4> sprites;
public:
    static std::string GetStaticType() {return "ATLAS";}
public:
    void SetTexture(const ObjectRef<ObjTexture>& newTex) {texture = newTex;}

    inline const IVec4& GetSpriteRect(uint16_t id) const;
    inline const std::map<uint16_t, IVec4>& GetSpriteRects() const {return sprites;}
    inline ObjectRef<ObjTexture>& GetTexture() {return texture;}

    void SetSpriteRect(uint16_t id, const IVec4& rect);
    void SetSpriteRect(uint16_t id, const IVec4&& rect);

    void SetSpriteRects(const std::map<uint16_t, IVec4> rects) {sprites = rects;}

    void RemoveSpriteRect(uint16_t id);

    virtual void Serialize(FieldContainer& container) override;
};

class ObjAtlasType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new ObjAtlas; }
    virtual std::string_view DisplayName() override {return "Atlas";}
};