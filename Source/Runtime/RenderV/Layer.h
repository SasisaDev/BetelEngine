#pragma once

#include "Vulkan/vkloader.h"

#include "LayerRef.h"

class IRenderLayer
{
protected:
    
public:
    static IRenderLayerRef* CreateRef();

    virtual std::string GetName() const {return "__EmptyNull";}

    virtual bool Initialize(){return false;}

    virtual void Prepare(IRenderLayerRef* previousLayer){}

    virtual void Render(IRenderLayerRef* layerRef, float DeltaTime){}

    bool Deinitialize(){return false;}
};