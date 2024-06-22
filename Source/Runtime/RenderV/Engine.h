#pragma once

#include <vector>
#include <optional>

#include "Composition.h"

struct RenderQueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    std::optional<uint32_t> computeFamily;

    bool IsComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value() && computeFamily.has_value();
    }
};

class IRenderEngine
{
protected:
    std::vector<IRenderLayer*> Layers;
    std::vector<IRenderComposition*> Compositions;

public:
    
    template <typename LayerType>
    uint32_t CreateLayer();
    IRenderLayer* GetLayer(uint32_t id);

    uint32_t CreateComposition();
    IRenderComposition* GetComposition(uint32_t id);
};