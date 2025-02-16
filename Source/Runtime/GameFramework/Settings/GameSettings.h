#pragma once

#include <Settings/Settings.h>

class GameSettings : public Settings
{
public:
    static std::string GetName() {return "Game"; }

    uint32_t PixelPerfectViewportWidth = 320, PixelPerfectViewportHeight = 180;
    bool CompensateAspectRatio = true;
    std::string GameTitle = "Betel Game";

    uint32_t MaxFPS = 120;

    virtual void Deserialize(INIFile& file) override {
        std::string value = file.GetValueOrDefault("General", "MaxFPS", "120");
        MaxFPS = std::stoi(value);
        GameTitle = file.GetValueOrDefault("General", "GameTitle", "Betel Game");

        // Pixel Perfect
        value = file.GetValueOrDefault("Pixel Perfect", "PixelPerfectViewportWidth", "320");
        PixelPerfectViewportWidth = std::stoi(value);
        value = file.GetValueOrDefault("Pixel Perfect", "PixelPerfectViewportHeight", "180");
        PixelPerfectViewportHeight = std::stoi(value);
        value = file.GetValueOrDefault("Pixel Perfect", "CompensateAspectRatio", "true");
        CompensateAspectRatio = (value == "true") ? true : false;
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
            {"CompensateAspectRatio", CompensateAspectRatio ? "true" : "false"}
        };

        iniFile.SetDomain("Pixel Perfect", PixelPefectMap);

        return iniFile;
    }
};