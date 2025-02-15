#pragma once

#include <string>
#include <string_view>
#include <cstdint>

// TODO
struct StringSanitizer
{
    static std::string_view LegalANSICharacters;

    static bool IsCompliant(const std::string &string)
    {
        for (char c : string)
        {
            if (LegalANSICharacters.find_first_of(c) == -1)
            {
                return false;
            }
        }
        return true;
    }

    static std::string Sanitize(const std::string &dirty)
    {   
        std::string output = dirty;
        size_t illegalCharacter = output.find_first_not_of(LegalANSICharacters);
        while(illegalCharacter != -1)
        {
            if(output[illegalCharacter] == ' ') {
                output[illegalCharacter] = '_';
            } else {
                output.erase(output.begin() + illegalCharacter);
            }

            illegalCharacter  = output.find_first_not_of(LegalANSICharacters);
        }
        return output;
    }
};

inline std::string_view StringSanitizer::LegalANSICharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_-";