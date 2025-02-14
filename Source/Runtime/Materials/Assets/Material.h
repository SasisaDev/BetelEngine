#pragma once

#include <memory>

#include <Object/ObjectRef.h>
#include <Object/Object.h>
#include <RenderV/Objects/Material.h>

#include "Shader.h"

class ObjMaterial : public Object
{
protected:
    ObjectRef<ObjShader> Shader;
    std::shared_ptr<IMaterial> pMaterial;
public:

    static std::string GetStaticType() {return "MAT";}
};

class ObjMaterialType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new ObjMaterial; }
    virtual bool CanSpawnIntoWorld() override {return false;}
    virtual std::string_view DisplayName() override {return "Material";}
};