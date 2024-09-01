#pragma once

#include <Log/Logger.h>
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

    template <typename LayerType>
    static std::shared_ptr<IShader> CreateShader(IRenderEngine* engine, std::vector<unsigned char> vertCode, std::vector<unsigned char> fragCode)
    {
        for(IRenderLayer* layer : engine->Layers)
        {
            if(layer->GetName() == LayerType::GetName())
            {
                return std::make_shared<IShader>(layer->GetRenderPass(), vertCode, fragCode);
            }
        }
        
        LOG(Error, LogRenderFactory, "Shader creation failed: Specified layer is not registered!");
        return nullptr;
    }

    static std::shared_ptr<IMaterial> CreateMaterial(IRenderEngine* engine, IShader* shader);
};