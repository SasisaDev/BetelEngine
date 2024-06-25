#pragma once

#include <RenderV/Layer.h>

class WorldRenderLayerRef : public IRenderLayerRef
{
public:

};

class WorldRenderLayer : public IRenderLayer
{

public:
    static IRenderLayerRef* CreateRef() {return new WorldRenderLayerRef;}

    virtual std::string GetName() const override {return "WorldRenderLayer";}
};