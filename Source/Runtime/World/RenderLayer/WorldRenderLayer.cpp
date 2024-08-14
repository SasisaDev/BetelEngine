#include "WorldRenderLayer.h"
#include <Engine/EngineDelegates.h>
#include <functional>

WorldRenderLayerRef::WorldRenderLayerRef()
{
    std::function<void(World*)> func;
    func = std::bind(&WorldRenderLayerRef::onWorldLoad, this, std::placeholders::_1);
    EngineDelegates::OnWorldLoad.AddBind(func);
}

WorldRenderLayerRef* WorldRenderLayerRef::SetWorld(World* newWorld)
{
    world = newWorld;
    return this;
}