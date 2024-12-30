#pragma once

#include "Vulkan/vkloader.h"

#include "LayerRef.h"

class IRenderLayer
{
    friend class IRenderEngine;
protected:
    VkRenderPass renderPass;
    std::string localName;
public:
    static IRenderLayerRef* CreateRef();

    inline VkRenderPass GetRenderPass() const {return renderPass;}
    virtual VkRenderPass GetCompositionCompatRenderPass() const {return renderPass;}

    static std::string GetStaticName() {return "__EmptyNull";}
    /* 
     * Returns local name specified from static name on creation. 
     * GetName() == LayerType::GetStaticName()
     */
    std::string GetName() {return localName;}

    virtual bool Initialize(VkDevice device){return false;}

    virtual void Prepare(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer){}

    virtual void Render(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer){}

    virtual bool Deinitialize(){return false;}
};