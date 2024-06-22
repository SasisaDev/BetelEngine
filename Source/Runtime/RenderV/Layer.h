#pragma once

#include "Vulkan/vkloader.h"

#include "LayerRef.h"

class IRenderLayer
{
protected:
    
public:
    static IRenderLayerRef* CreateRef();

    virtual std::string GetName() const {return "__EmptyNull";}

    virtual bool Initialize();

    virtual void Prepare(IRenderLayer* previousLayer);

    virtual void Render(float DeltaTime);

    bool Deinitialize();
};