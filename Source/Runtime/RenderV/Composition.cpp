#include "Composition.h"

IRenderComposition::IRenderComposition()
    : Layers()
{

}

bool IRenderComposition::Initialize(IRenderCompositionInitializer* initializer)
{
    switch(initializer->GetType())
    {
        case ERenderCompositionType::RENDER_COMPOSITION_TYPE_SURFACE:
            return InitializeWithSurface(static_cast<RenderCompositionInitializerSurface*>(initializer));
        break;
        case ERenderCompositionType::RENDER_COMPOSITION_TYPE_IMAGE:
            return InitializeWithImage(static_cast<RenderCompositionInitializerImage*>(initializer));
        break;
        default:
            return false;
        break;
    }
}

bool IRenderComposition::InitializeWithSurface(RenderCompositionInitializerSurface* initializer)
{
    return false;
}

bool IRenderComposition::InitializeWithImage(RenderCompositionInitializerImage* initializer)
{
    return false;
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

