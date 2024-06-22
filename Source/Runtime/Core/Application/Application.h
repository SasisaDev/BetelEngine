#pragma once

#include <cstdint>
#include <vector>

#include "../Window/Window.h"

class Application
{
protected:
    std::vector<Window> Windows;

    bool bShouldClose = false;
public:
    Application();

    window_t CreateWindow(WindowCreateInfo& createInfo);

    void ApplicationLoop();
};