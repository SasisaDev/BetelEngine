#pragma once

#include "Engine.h"
#include "Objects/Material.h"

#include <memory>

class IRenderFactory
{
public:
    static IRenderEngine* CreateEngine();

    template <typename LayerType, typename LayerRefType = IRenderLayerRef>
    static LayerRefType* CreateLayerRef(IRenderEngine* engine)
    {
        assert(engine != nullptr && "engine == nullptr");

        IRenderLayerRef* ref = LayerType::CreateRef();
        for(IRenderLayer* layer : engine->GetLayers())
        {
            if(typeid(*layer) == typeid(LayerType))
            {
                ref->SetParentLayer(layer);
                return dynamic_cast<LayerRefType*>(ref);
            }
        }
        
        assert(!"IRenderFactory::CreateLayerRef(IRenderEngine) points to non-registered layer type, which should never happen.");
        return dynamic_cast<LayerRefType*>(ref);
    }

    static std::shared_ptr<IShader> CreateShader(IRenderEngine* engine, IRenderLayer* layer, std::vector<unsigned char> vertCode, std::vector<unsigned char> fragCode);
    static std::shared_ptr<IMaterial> CreateMaterial(IRenderEngine* engine, IShader* shader);
};