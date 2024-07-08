#include "Engine.h"

#include <cassert>
#include <set>

#include <Log/Logger.h>

VkPhysicalDevice IRenderEngine::InternalPickPhysDevice(std::vector<VkPhysicalDevice> &PhysicalDevices)
{
    for(int i = 0; i < PhysicalDevices.size(); i++)
    {
        if(IRenderUtility::FindQueueFamilies(PhysicalDevices[i]).graphicsFamily.has_value())
        {
            return PhysicalDevices[i];
        }
    }

    LOG(Fatal, LogVulkan, "Found no physical devices supporting Vulkan on current system.");
    return nullptr;
}

bool IRenderEngine::Initialize(std::vector<const char*> extensions, std::vector<const char*> layers)
{
    // Initialize Vulkan Loader
    VkResult result = vkloader::InitializeLoader();
    if(result != VK_SUCCESS)
    {
        LOG(Fatal, LogVulkan, "Failed initializing vulkan loader");
        return false;
    }

    // Create Vulkan Instance
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = 0;
    appInfo.pEngineName = "Konide";
    appInfo.pApplicationName = "Arteria";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pNext = 0;
    instanceInfo.pApplicationInfo = &appInfo;

    std::vector<const char*> exts/* = InternalAssembleExtensions()*/;
    exts.insert(exts.end(), extensions.begin(), extensions.end());

    instanceInfo.enabledExtensionCount = exts.size();
    instanceInfo.ppEnabledExtensionNames = exts.data();

    std::vector<const char*> layrs/* = InternalAssembleLayers()*/;
    layrs.insert(layrs.end(), layers.begin(), layers.end());

    instanceInfo.enabledLayerCount = layrs.size();
    instanceInfo.ppEnabledLayerNames = layrs.data();

    result = vkloader::vkCreateInstance(&instanceInfo, nullptr, &instance);
    if(result != VK_SUCCESS)
    {
        LOG(Fatal, LogVulkan, "Failed creating VkInstance");
        return false;
    }

    // Pick Physical Device
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        LOG(Fatal, LogVulkan, "failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    physDevice = DelegatePickPhysDevice(devices);



    return true;
}

void IRenderEngine::CreateDevice(std::vector<const char*> devExtensions, std::vector<const char*> devLayers)
{
    // Ensure all family indices are picked
    if(!queueFamilyIndices.presentFamily.has_value())
    {
        LOG(Warning, LogVulkan, "Presentation family index was not picked before device creation. Presentation may be unavailable. Are there any Surface Compositions?");
        queueFamilyIndices.presentFamily = queueFamilyIndices.graphicsFamily;
    }

    if(!queueFamilyIndices.computeFamily.has_value())
    {
        LOG(Warning, LogVulkan, "Presentation family index was not picked before device creation. Compute shaders would not be unavailable.");
    }

    // Create Device Queue
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value(), queueFamilyIndices.computeFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.pNext = nullptr;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    // Create Device
    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    std::vector<const char*> extensions/* = InternalAssembleDeviceExtensions()*/;
    extensions.insert(extensions.end(), devExtensions.begin(), devExtensions.end());
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = extensions.data();

    std::vector<const char*> layers = {};
    layers.insert(layers.end(), devLayers.begin(), devLayers.end());
    deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
    deviceCreateInfo.ppEnabledLayerNames = layers.data();

    if (vkloader::vkCreateDevice(physDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
        LOG(Fatal, LogVulkan, "Failed creating device");
    }

    vkGetDeviceQueue(device, queueFamilyIndices.graphicsFamily.value(), 0, &graphicsQueue);
    if(queueFamilyIndices.presentFamily.has_value()) 
    {   
        vkGetDeviceQueue(device, queueFamilyIndices.presentFamily.value(), 0, &presentQueue);
    }
    if(queueFamilyIndices.computeFamily.has_value()) 
    {   
        vkGetDeviceQueue(device, queueFamilyIndices.computeFamily.value(), 0, &computeQueue);
    }
}

IRenderLayer* IRenderEngine::GetLayer(uint32_t id)
{
    return Layers[id];
}

uint32_t IRenderEngine::CreateComposition(IRenderCompositionInitializer* initializer)
{
    IRenderComposition* composition = new IRenderComposition();
    composition->Initialize(initializer);
    
    if(!queueFamilyIndices.IsComplete())
    {
        queueFamilyIndices = IRenderUtility::FindQueueFamilies(physDevice, composition->GetSurface());
    }

    Compositions.push_back(composition);
    return Compositions.size()-1;
}

IRenderComposition* IRenderEngine::GetComposition(uint32_t id)
{
    return Compositions[id];
}

void IRenderEngine::Render()
{
    for(size_t compositionId = 0; compositionId < Compositions.size(); compositionId++)
    {
        Compositions[compositionId]->Render(0);
    }
}

// Debug implementations
#if !defined(NDEBUG)
void IRenderEngine::CreateDebugMessenger(PFN_vkDebugUtilsMessengerCallbackEXT callback, void* userData)
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = callback;
    createInfo.pUserData = userData;

    vkCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);
}
#endif