#pragma once

class EditorMode
{

public:

    virtual void OnEnabled(){}
    virtual void Tick(float deltaTime) {}
    virtual void OnDisabled(){}
};