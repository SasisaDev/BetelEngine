#pragma once

#include <string>
#include <optional>

class Text
{
    // Domain.Subdomain.Name
    std::string Domain;
    std::string Subdomain;
    std::string Name;

    std::optional<std::string> TranslationMemoize;
public:
    Text() {}
    Text(std::string domain, std::string subdomain, std::string name) : Domain(domain), Subdomain(subdomain), Name(name) {}
    
    std::string Get();

    std::string operator *() {
        return Get();
    }
};