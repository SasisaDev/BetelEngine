#include "TestEntity.h"
#include <RenderV/Utility.h>

void EntityRenderProxyTest::Render(VkCommandBuffer cmdBuffer)
{
    IRenderUtility::BeginDebugLabel(cmdBuffer, "Test");

    IRenderUtility::EndDebugLabel(cmdBuffer);
}