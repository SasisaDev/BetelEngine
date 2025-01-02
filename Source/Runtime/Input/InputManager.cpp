#include "InputManager.h"
#include <Log/Logger.h>
#include <Core/Application/Application.h>

void InputManager::InternalHandleEvent(SDL_Event e) {
    InputEvent event;

    switch(e.type) {
        case SDL_KEYDOWN:
            event.KeyName = SDL_GetKeyName(e.key.keysym.sym);
            break;
        case SDL_MOUSEMOTION:
            //LOGF(Log, LogInput, "Mouse motion: %i, %i", e.motion.xrel, e.motion.yrel);
            event.MouseAxisX = e.motion.xrel;
            event.MouseAxisY = e.motion.yrel;
            break;
    };

    OnInputEvent.Broadcast(event);
}