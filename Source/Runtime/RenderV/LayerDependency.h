#pragma once

#include <vector>
#include <string>
#include <map>

class IRenderLayerRef;

class RenderLayerDependency
{
protected:
    std::map<IRenderLayerRef*, std::vector<std::string>> DependencyList;
public:

    inline const std::map<IRenderLayerRef*, std::vector<std::string>>& GetDependencyList() const { return DependencyList; }

    void AddDependency(IRenderLayerRef* Source, std::string Dependency);
};