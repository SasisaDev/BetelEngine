#include "Layer.h"
#include <RenderV/Utility.h>

IRenderLayer::~IRenderLayer()
{
    vkDestroyRenderPass(IRenderUtility::GetDevice(), renderPass, nullptr);
}