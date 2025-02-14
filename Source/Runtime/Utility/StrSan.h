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
        return {};
    }
};

inline std::string_view StringSanitizer::LegalANSICharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_-";