#pragma once

#include "Vulkan/vkloader.h"

#include "LayerDependency.h"

class RenderLayer
{
protected:

public:

    virtual bool Initialize(RenderLayerDependency& DependencyList);

    virtual void Prepare(RenderLayer* previousLayer);

    virtual void Render(float DeltaTime);

    bool Deinitialize();
};