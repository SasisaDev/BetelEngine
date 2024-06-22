#pragma once

#include <vector>
#include <string>
#include <map>

class IRenderLayer;

class RenderLayerDependency
{
protected:
    std::map<IRenderLayer*, std::vector<std::string>> DependencyList;
public:

    inline const std::map<IRenderLayer*, std::vector<std::string>>& GetDependencyList() const { return DependencyList; }

    void AddDependency(IRenderLayer* Source, std::string Dependency);
};