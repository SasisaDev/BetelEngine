#pragma once
#include <string>
#include <cstdint>

struct InputEvent {
    bool Shift : 1 = false;
    bool Ctrl : 1 = false; 
    bool Alt : 1 = false;
    bool Caps : 1 = false;

    std::string KeyName;
    uint8_t MouseButton; 
    
    // Was key or button unpressed
    bool IsUp = false;
    // Is action related to mouse
    bool IsMouse = false;
    // Is action related to gamepas
    bool IsGamepad = false;
    // Is action related to key, not an axis
    bool IsKey = false;

    float AxisX = 0;
    float AxisY = 0;
    float MouseX = 0;
    float MouseY = 0;
};