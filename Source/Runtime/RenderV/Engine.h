#pragma once

#include <vector>
#include <optional>

#include "Utility.h"
#include "Composition.h"

#include "Vulkan/vkloader.h"

class IRenderEngine
{
protected:
    std::vector<IRenderLayer*> Layers;
    std::vector<IRenderComposition*> Compositions;

    VkInstance instance;
    VkPhysicalDevice physDevice;
    VkDevice device;

#   if !defined(NDEBUG)
    VkDebugUtilsMessengerEXT debugMessenger;
#   endif

    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkQueue computeQueue;

    VkSemaphore aquireSemaphore;
    VkSemaphore submitSemaphore;
    VkCommandPool cmdPool;
    VkCommandBuffer cmdBuffer;

    RenderQueueFamilyIndices queueFamilyIndices;

    VkPhysicalDevice (*DelegatePickPhysDevice)(std::vector<VkPhysicalDevice> &PhysicalDevices) = &IRenderEngine::InternalPickPhysDevice;

    uint32_t RenderFeatureFlags = 0;

    static VkPhysicalDevice InternalPickPhysDevice(std::vector<VkPhysicalDevice> &PhysicalDevices);

    std::vector<const char*> InternalAssembleExtensions();
    std::vector<const char*> InternalAssembleLayers();
    
    std::vector<const char*> InternalAssembleDeviceExtensions();
    std::vector<const char*> InternalAssembleDeviceLayers();

public:

    bool Initialize(std::vector<const char*> extensions = {}, std::vector<const char*> layers = {});
    void CreateDevice(std::vector<const char*> devExtensions = {}, std::vector<const char*> devLayers = {});

    inline VkInstance GetInstance() const {return instance;}
    
    template <typename LayerType>
    uint32_t CreateLayer();
    IRenderLayer* GetLayer(uint32_t id);
    std::vector<IRenderLayer*> GetLayers() const {return Layers;}

    uint32_t CreateComposition(IRenderCompositionInitializer* initializer);
    IRenderComposition* GetComposition(uint32_t id);

    void Render();
public:
// Debug Functions
#   if !defined(NDEBUG)
    void CreateDebugMessenger(PFN_vkDebugUtilsMessengerCallbackEXT callback, void* userData);
#endif
};

template <typename LayerType>
inline uint32_t IRenderEngine::CreateLayer()
{
    Layers.push_back(new LayerType);
    return Layers.size()-1;
}