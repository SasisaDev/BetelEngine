#pragma once

struct InputEvent {
    bool Shift : 1 = false;
    bool Ctrl : 1 = false; 
    bool Alt : 1 = false;
    bool Caps : 1 = false;

    std::string KeyName;

    float MouseAxisX = 0;
    float MouseAxisY = 0;
};