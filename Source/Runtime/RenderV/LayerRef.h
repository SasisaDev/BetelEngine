#pragma once

#include "Vulkan/vkloader.h"
#include "DependencyList.h"
#include <cassert>

class IRenderLayer;

class IRenderLayerRef
{
    IRenderLayer* parentLayer = nullptr;
public:
    void SetParentLayer(IRenderLayer* newParentLayer)
    {
        assert((parentLayer == nullptr) && "Layer ref must never be reparanted under normal conditions");

        parentLayer = newParentLayer;
    }
    inline IRenderLayer* GetParentLayer() const {return parentLayer;}

    virtual bool Initialize(VkDevice device, RenderDependencyList<IRenderLayerRef>& DependencyList){return false;}
    virtual bool Recreate() {return false;}

    virtual void Prepare(VkDevice device, IRenderLayerRef* previousLayer){}

    virtual void Render(VkDevice device){}

    bool Deinitialize(VkDevice device) {return false;}
};