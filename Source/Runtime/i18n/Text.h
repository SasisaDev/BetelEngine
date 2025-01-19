#pragma once

#include <string>
#include <optional>

class Text
{
    static std::string NOENGINE;
    // Domain.Subdomain.Name
    std::string Domain;
    std::string Subdomain;
    std::string Name;

    std::optional<std::string> TranslationMemoize;
public:
    Text() {}
    Text(std::string domain, std::string subdomain, std::string name) : Domain(domain), Subdomain(subdomain), Name(name) {}
    Text(std::string fullID) {
        // TODO
    }
    
    std::string& Get();

    std::string& operator *() {
        return Get();
    }
};