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
    inline const IVec4& GetSpriteRect(uint16_t id) const;
    inline ObjectRef<ObjTexture>& GetTexture() {return texture;}

    inline void SetSpriteRect(uint16_t id, const IVec4& rect);
    inline void SetSpriteRect(uint16_t id, const IVec4&& rect);

    inline void RemoveSpriteRect(uint16_t id);

    virtual void Serialize(FieldContainer& container) override;
};

class ObjAtlasType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new ObjAtlas; }
};