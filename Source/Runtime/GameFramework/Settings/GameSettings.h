#pragma once

#include <Settings/Settings.h>

class GameSettings : public Settings
{
public:
    static std::string GetName() {return "Game"; }

    uint32_t PixelPerfectViewportWidth = 320, PixelPerfectViewportHeight = 180;
};