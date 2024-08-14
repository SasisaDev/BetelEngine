#pragma once

#include <Settings/Settings.h>

class GameSettings : public Settings
{
public:
    GameSettings() : Settings("Game") {}
};