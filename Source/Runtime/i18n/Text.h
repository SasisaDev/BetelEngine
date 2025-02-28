#pragma once

#include <string>
#include <optional>

class Text
{
    std::string_view ID;

    std::optional<std::string> TranslationMemoize;
public:
    Text() {}
    Text(const std::string_view& id) : ID(id) {}
    
    const std::string& Get();

    const std::string& operator *() {
        return Get();
    }
};