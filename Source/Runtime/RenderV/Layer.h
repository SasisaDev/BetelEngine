#pragma once

#include "Vulkan/vkloader.h"

#include "LayerRef.h"

class IRenderLayer
{
protected:
    
public:
    static IRenderLayerRef* CreateRef();

    virtual std::string GetName() const {return "__EmptyNull";}

    virtual bool Initialize(VkDevice device){return false;}

    virtual void Prepare(VkDevice device, IRenderLayerRef* previousLayer){}

    virtual void Render(VkDevice device, IRenderLayerRef* layerRef){}

    bool Deinitialize(){return false;}
};