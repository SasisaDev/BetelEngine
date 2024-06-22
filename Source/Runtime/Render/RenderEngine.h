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

class RenderEngine
{
protected:
    std::vector<RenderComposition*> Compositions;

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

    VkPhysicalDevice (*DelegatePickPhysDevice)(std::vector<VkPhysicalDevice> &PhysicalDevices) = &RenderEngine::InternalPickPhysDevice;

    static VkPhysicalDevice InternalPickPhysDevice(std::vector<VkPhysicalDevice> &PhysicalDevices);

    std::vector<const char*> InternalAssembleExtensions();
    std::vector<const char*> InternalAssembleLayers();
    
    std::vector<const char*> InternalAssembleDeviceExtensions();
    std::vector<const char*> InternalAssembleDeviceLayers();

public:

    bool Initialize(std::vector<const char*> extensions = {}, std::vector<const char*> layers = {});

    void CreateDebugMessenger(PFN_vkDebugUtilsMessengerCallbackEXT callback, void* userData);

    uint32_t CreateComposition(IRenderCompositionInitializer* initializer);
    RenderComposition* GetComposition(uint32_t idx);

    void Render(float DeltaTime);

    bool Deinitialize();
};