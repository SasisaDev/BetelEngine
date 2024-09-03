#pragma once

#include <World/Entity.h>

class EntityRenderProxyTest : public EntityRenderProxy
{
    Entity* Parent;
public:
    EntityRenderProxyTest(Entity* DefaultParent) : EntityRenderProxy(DefaultParent) {}
    virtual void Update() override{}
    virtual void Render(VkCommandBuffer cmdBuffer) override;
};

class EntityTest : public Entity
{
public:

    virtual EntityRenderProxy* CreateRenderProxy() override {return RenderProxy = new EntityRenderProxyTest(this);}
};