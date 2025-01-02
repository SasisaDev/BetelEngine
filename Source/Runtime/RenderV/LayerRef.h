#pragma once

#include "Vulkan/vkloader.h"
#include "DependencyList.h"
#include <cassert>

class IRenderLayer;
class IRenderComposition;

class IRenderLayerRef
{
    IRenderLayer* parentLayer = nullptr;
    IRenderComposition* parentComposition = nullptr;
public:
    void SetParentLayer(IRenderLayer* newParentLayer)
    {
        assert((parentLayer == nullptr) && "Layer ref must never be reparanted under normal conditions");

        parentLayer = newParentLayer;
    }
    inline IRenderLayer* GetParentLayer() const {return parentLayer;}

    void SetParentComposition(IRenderComposition* newParentComp)
    {
        assert((parentComposition == nullptr) && "Layer ref's composition must never be reparanted under normal conditions");

        parentComposition = newParentComp;
    }
    inline IRenderComposition* GetParentComposition() const {return parentComposition;}

    virtual bool Initialize(VkDevice device, RenderDependencyList<IRenderLayerRef>& DependencyList){return false;}
    virtual bool Recreate() {return false;}

    virtual void Prepare(VkDevice device, IRenderLayerRef* previousLayer){}

    virtual void Render(VkDevice device){}

    virtual bool Deinitialize(VkDevice device) {return false;}

    template <typename LayerT>
    LayerT* PushInto(LayerT** layerPtr) {*layerPtr = dynamic_cast<LayerT*>(this); return dynamic_cast<LayerT*>(this);}
};