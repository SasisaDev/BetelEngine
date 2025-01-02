#pragma once

#include <map>
#include <string>
#include <cassert>

typedef std::map<std::string, std::string> INIMap;

class INIFile {
    std::map<std::string, INIMap> DataMap;
public:

    bool HasDomain(std::string domain) {
        return DataMap.find(domain) != DataMap.end();
    }

    INIMap GetDomain(std::string domain) {
        return DataMap[domain];
    }

    void SetDomain(std::string domain, INIMap data) {
        DataMap[domain] = data;
    }

    std::string GenerateFileBuffer() {
        // TODO: Generate file data
        std::string buffer;

        for(auto domainIterator = DataMap.begin();domainIterator != DataMap.end(); domainIterator++)
        {
            buffer += "[" + domainIterator->first + "]\n";
            for(auto keyIterator = domainIterator->second.begin(); keyIterator != domainIterator->second.end(); keyIterator++)
            {
                std::string formatedString = keyIterator->second;
                size_t formatPosition = 0;
                while((formatPosition = formatedString.find('\n', formatPosition)) != std::string::npos) {
                    formatedString.insert(formatPosition, "\\");
                    formatPosition += 2;
                }
                buffer += keyIterator->first + "=" + formatedString + "\n";
            }
        }

        return buffer;
    }

    static INIFile LoadFromMemory(char* buffer, size_t size) {
        INIFile file;
        
        std::string domain, key, value;
        bool pDomain = true, pValue = false; 
        bool comment = false;
        bool multiline = false;

        for(size_t i = 0; i < size; i++){
            // We don't care about Window's endline \r byte
            if(buffer[i] == '\r') {
                continue;
            }
            // Line end; Submit data
            if(buffer[i] == '\0' || buffer[i] == '\n') {
                comment = false;
                
                // Don't push if multiline is still enabled
                if(multiline) {
                    multiline = false;
                    continue;
                }

                // This check guards against broken pairs and domain defining sequence
                if(domain.size() > 0 && key.size() > 0 && value.size() > 0) {
                    file.DataMap[domain][key] = value;
                    key.clear();
                    value.clear();
                    pValue = false;
                }
                continue;
            }
            
            // Don't do anything if it's comment
            if(comment) {
                continue;
            }

            // Comment
            if(buffer[i] == ';') {
                comment = true;
                continue;
            }

            // Multiline value or '\\' character
            if(buffer[i] == '\\' && pValue) {
                if(buffer[i+1]=='\\') {
                    value.push_back('\\');
                    i += 1;
                    continue;
                }
                multiline = true;
                value.push_back('\n');
                continue;
            }

            // Domain defining sequence 
            if(buffer[i] == '[') {
                domain.clear();
                pDomain = true;
                continue;
            } else if(buffer[i] == ']') {
                pDomain = false;
                continue;
            }

            // Assign value
            if(buffer[i] == '=') {
                pValue = true;
                continue;
            }

            // Add char to the string
            if(pDomain) {
                domain.push_back(buffer[i]);
            } else if (pValue) {
                value.push_back(buffer[i]);
            } else {
                key.push_back(buffer[i]);
            }
        }

        return file;
    }
};