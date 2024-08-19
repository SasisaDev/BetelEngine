#pragma once

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

    virtual bool Initialize(RenderDependencyList<IRenderLayerRef>& DependencyList){return false;}
    virtual bool Recreate() {return false;}

    virtual void Prepare(IRenderLayerRef* previousLayer){}

    virtual void Render(float DeltaTime){}

    bool Deinitialize() {return false;}
};