#pragma once
#include <cstdint>

enum WindowEventType
{
    WINDOW_EVENT_RESIZE,
    WINDOW_EVENT_MINIMIZE,
    WINDOW_EVENT_MAXIMIZE,
    WINDOW_EVENT_FULLSCREEN
};

class WindowEventPayload {public: WindowEventType type; };

class WindowEventPayloadResize : public WindowEventPayload {
public:
    WindowEventPayloadResize(uint32_t w, uint32_t h) : width(w), height(h) {type=WINDOW_EVENT_RESIZE;}
    uint32_t width, height;
};

class WindowEventPayloadFullscreen : public WindowEventPayload {
public:
    WindowEventPayloadFullscreen(bool is, uint32_t w, uint32_t h) : isFullscreen(is), width(w), height(h) {type=WINDOW_EVENT_RESIZE;}
    bool isFullscreen;
    uint32_t width, height;
};