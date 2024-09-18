#include "Panel.h"

void PanelWidget::Render(VkCommandBuffer cmdBuffer)
{
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, brush.shader.Load()->GetShader()->GetPipeline());

    vkCmdDraw(cmdBuffer, 3, 1, 0, 0);
}