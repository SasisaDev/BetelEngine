#include "Layer.h"
#include <RenderV/Utility.h>

bool IRenderLayer::Deinitialize()
{
    if(renderPass != VK_NULL_HANDLE){
        vkDestroyRenderPass(IRenderUtility::GetDevice(), renderPass, nullptr);
    }
    
    return true;
}

IRenderLayer::~IRenderLayer()
{

}