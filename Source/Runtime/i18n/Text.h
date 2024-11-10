#pragma once

#include <string>

class Text
{
    // Domain.Subdomain.Name
    std::string Domain;
    std::string Subdomain;
    std::string Name;
public:
    
    std::string Get();

    std::string operator *() {
        return Get();
    }
};