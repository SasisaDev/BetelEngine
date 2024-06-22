#pragma once

#include "LayerDependency.h"
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

    virtual bool Initialize(RenderLayerDependency& DependencyList);

    virtual void Prepare(IRenderLayerRef* previousLayer);

    virtual void Render(float DeltaTime);

    bool Deinitialize();
};