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

    VkDebugUtilsMessengerEXT debugMessenger;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

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

    inline VkInstance GetInstance() const {return instance;}
    
    template <typename LayerType>
    uint32_t CreateLayer();
    IRenderLayer* GetLayer(uint32_t id);

    uint32_t CreateComposition();
    IRenderComposition* GetComposition(uint32_t id);

    void Render();
};