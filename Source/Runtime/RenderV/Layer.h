#pragma once

#include "Vulkan/vkloader.h"

#include "LayerRef.h"

class IRenderLayer
{
protected:
    VkRenderPass renderPass;
public:
    static IRenderLayerRef* CreateRef();

    inline VkRenderPass GetRenderPass() const {return renderPass;}

    static std::string GetName() {return "__EmptyNull";}

    virtual bool Initialize(VkDevice device){return false;}

    virtual void Prepare(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer){}

    virtual void Render(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer){}

    virtual bool Deinitialize(){return false;}
};