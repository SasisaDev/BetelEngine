#include "Shader.h"

#include <RenderV/Utility.h>

IShader::IShader(VkRenderPass renderPass, const char* vertexData, const char* fragmentData)
{
    
}

IShader::~IShader()
{
    vkDestroyPipelineLayout(IRenderUtility::GetDevice(), pipelineLayout, nullptr);
}