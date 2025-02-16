#pragma once

class Window;

class ImGuiRenderable
{
public:

    virtual void OnGUI(Window* window){}
};