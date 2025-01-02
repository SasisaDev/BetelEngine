#pragma once

#include <Settings/Settings.h>

class GameSettings : public Settings
{
public:
    static std::string GetName() {return "Game"; }

    uint32_t PixelPerfectViewportWidth = 320, PixelPerfectViewportHeight = 180;
    std::string GameTitle = "Betel Game";

    uint32_t MaxFPS = 120;

    virtual void Deserialize(INIFile& file) override {
        std::string maxFPS = file.GetDomain("General")["MaxFPS"];
        MaxFPS = std::stoi(maxFPS);
    }

    virtual INIFile Serialize() override {
        INIFile iniFile;

        INIMap GeneralMap = {
            {"GameTitle", GameTitle},
            {"MaxFPS", std::to_string(MaxFPS)},
        };

        iniFile.SetDomain("General", GeneralMap);
        
        INIMap PixelPefectMap = {
            {"PixelPerfectViewportWidth", std::to_string(PixelPerfectViewportWidth)},
            {"PixelPerfectViewportHeight", std::to_string(PixelPerfectViewportHeight)},
        };

        iniFile.SetDomain("Pixel Perfect", PixelPefectMap);

        return iniFile;
    }
};