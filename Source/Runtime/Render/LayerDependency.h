#pragma once

#include <vector>
#include <string>
#include <map>

class RenderLayer;

class RenderLayerDependency
{
protected:
    std::map<RenderLayer*, std::vector<std::string>> DependencyList;
public:

    inline const std::map<RenderLayer*, std::vector<std::string>>& GetDependencyList() const { return DependencyList; }

    void AddDependency(RenderLayer* Source, std::string Dependency);
};