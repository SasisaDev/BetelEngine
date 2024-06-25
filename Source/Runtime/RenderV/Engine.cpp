#include "Engine.h"

#include <cassert>

#include <Log/Logger.h>

VkPhysicalDevice IRenderEngine::InternalPickPhysDevice(std::vector<VkPhysicalDevice> &PhysicalDevices)
{
    return VK_NULL_HANDLE;
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
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = 0;
    appInfo.pEngineName = "Deiri";
    appInfo.pApplicationName = "Deiri Game";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo instanceInfo;
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

    return true;
}

IRenderLayer* IRenderEngine::GetLayer(uint32_t id)
{
    return Layers[id];
}

uint32_t IRenderEngine::CreateComposition()
{
    Compositions.push_back(new IRenderComposition());
    return Compositions.size()-1;
}

IRenderComposition* IRenderEngine::GetComposition(uint32_t id)
{
    return Compositions[id];
}

void IRenderEngine::Render()
{
    for(int compositionId = 0; compositionId < Compositions.size(); compositionId++)
    {
        Compositions[compositionId]->Render(0);
    }
}