#pragma once

#include <map>
#include <string>

class INIFile {
    std::map<std::string, std::map<std::string, std::string>> DataMap;
public:

    bool HasDomain(std::string domain) {
        return DataMap.find(domain) != DataMap.end();
    }

    std::map<std::string, std::string> GetDomain(std::string domain) {
        return DataMap[domain];
    }

    void SetDomain(std::string domain, std::map<std::string, std::string> data) {
        DataMap[domain] = data;
    }

    // Call this without second argument to get size of the buffer you need to allocate
    void GenerateFileBuffer(size_t* size, unsigned char* buffer = nullptr) {
        // TODO: Generate file data
    }

    static INIFile* LoadFromMemory(char* buffer, size_t size) {
        INIFile* file = new INIFile;
        
        std::string domain, key, value;
        bool pDomain = true, pValue = false; 
        bool comment = false;
        bool multiline = false;

        for(size_t i = 0; i < size; i++){
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
                    file->DataMap[domain][key] = value;
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