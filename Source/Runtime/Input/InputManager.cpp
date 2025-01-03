#include "InputManager.h"
#include <Log/Logger.h>
#include <Core/Application/Application.h>

void InputManager::InternalHandleEvent(SDL_Event e) {
    InputEvent event;

    switch(e.type) {
        case SDL_KEYUP:
            event.IsUp = true;
            event.KeyName = SDL_GetKeyName(e.key.keysym.sym);
            break;
        case SDL_KEYDOWN:
            if(!e.key.repeat) {
                event.KeyName = SDL_GetKeyName(e.key.keysym.sym);
            }
            break;
        case SDL_MOUSEMOTION:
            event.IsMouse = true;
            event.AxisX = e.motion.xrel;
            event.AxisY = e.motion.yrel;
            event.MouseX = e.motion.x;
            event.MouseY = e.motion.y;
            break;
        case SDL_MOUSEBUTTONUP:
            event.IsUp = true;
        case SDL_MOUSEBUTTONDOWN:
            event.IsMouse = true;
            event.IsKey = true;
            event.MouseButton = e.button.button;
            event.MouseX = e.motion.x;
            event.MouseY = e.motion.y;
            break;
    };

    OnInputEvent.Broadcast(event);
}