#pragma once

#include <RenderV/Layer.h>

class World;

class WorldRenderLayerRef : public IRenderLayerRef
{
protected:
    World* world;
public:
    WorldRenderLayerRef();

    WorldRenderLayerRef* SetWorld(World* newWorld);

    void onWorldLoad(World* loadedWorld){}
};

class WorldRenderLayer : public IRenderLayer
{
public:
    static IRenderLayerRef* CreateRef() {return new WorldRenderLayerRef;}

    virtual std::string GetName() const override {return "WorldRenderLayer";}
};