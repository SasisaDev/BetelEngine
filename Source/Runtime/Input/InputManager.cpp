#include "InputManager.h"
#include <Log/Logger.h>
#include <Core/Application/Application.h>

void InputManager::InternalHandleEvent(SDL_Event e) {
    // Should not dispatch any events if the game is out of focus
    if(!GApplication || !GApplication->GetEngine() || !GApplication->GetEngine()->GetGameFocused()) {
        return;
    }

    switch(e.type) {
        case SDL_KEYDOWN:
            LOGF(Log, LogInput, "Key down: %s", SDL_GetKeyName(e.key.keysym.sym));
            break;
        case SDL_MOUSEMOTION:
            //LOGF(Log, LogInput, "Mouse motion: %i, %i", e.motion.xrel, e.motion.yrel);
            break;
    };
}