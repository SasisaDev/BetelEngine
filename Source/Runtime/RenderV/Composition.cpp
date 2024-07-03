#include "Composition.h"

void RenderCompositionInitializerSurface::Initialize(IRenderComposition* composition) 
{
    composition->surface = surface;
}

void RenderCompositionInitializerImage::Initialize(IRenderComposition* composition) 
{
    
}

IRenderComposition::IRenderComposition()
    : Layers()
{

}

bool IRenderComposition::Initialize(IRenderCompositionInitializer* initializer)
{
    compositionType = initializer->GetType();

    initializer->Initialize(this);

    return true;
}

uint32_t IRenderComposition::AddLayerRef(IRenderLayerRef* ref)
{  
    Layers.push_back(ref);
    return Layers.size() - 1;
}

std::pair<uint32_t, uint32_t> IRenderComposition::AddLayerRefs(std::vector<IRenderLayerRef*> &refs)
{  
    uint32_t firstID = Layers.size();
    Layers.insert(Layers.end(), refs.begin(), refs.end());
    return std::pair<uint32_t, uint32_t>(firstID, Layers.size() - 1);
}

void IRenderComposition::Render(float DeltaTime)
{
    IRenderLayerRef* previousLayer = nullptr;

    for(int layerRefId = 0; layerRefId < Layers.size(); layerRefId++)
    {
        Layers[layerRefId]->GetParentLayer()->Prepare(previousLayer);

        previousLayer = Layers[layerRefId];
    }

    previousLayer = nullptr;
    for(int layerRefId = 0; layerRefId < Layers.size(); layerRefId++)
    {
        Layers[layerRefId]->GetParentLayer()->Render(previousLayer, 0);

        previousLayer = Layers[layerRefId];
    }
}