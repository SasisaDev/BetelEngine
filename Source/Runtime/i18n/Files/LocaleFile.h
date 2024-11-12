#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <Platform/Platform.h>

class LocaleFile 
{
    friend class TextManager;

    std::vector<IFile*> fileHandles;
    std::string localeID;
    std::string localeName;

    std::unordered_map<std::string, std::string> LocaleMap;
public:
    void Load(std::vector<IFile*> files = {}){
        if(files.size() > 0) {
            fileHandles = files;
        }

        for(IFile* file : fileHandles) {
            if(!file->IsOpen()) {
                LOG(Error, LogI18N, "Locale file was not found");
                continue;
            }

            // Read buffers
            char *cur = nullptr;
            std::string key, value;
            bool recordingValue = false;
            while((cur = file->Fetch(1)) != nullptr) {
                if(cur[0] == '=') {
                    recordingValue = true;
                    continue;
                } else if(cur[0] == '\n' || cur[0] == '\r' || cur[0] == '\0' || cur[0] == EOF) {
                    LocaleMap[key] = value;
                    key.clear();
                    value.clear();
                    recordingValue = false;
                    continue;
                }

                if(recordingValue) {
                    value.push_back(cur[0]);
                } else {
                    key.push_back(cur[0]);
                }
            }

            // In case no stop byte is read
            if(!key.empty() || !value.empty()) {
                LocaleMap[key] = value;
            }
        }
    }

    void Free(){LocaleMap.clear();}

    std::string Get(std::string key) {
        // TODO: Make more verbose
        if(LocaleMap.find(key) == LocaleMap.end()) {
            LOG(Warning, LogI18N, "Key not found");
            return key;
        }

        return (LocaleMap.find(key))->second;
    }
};