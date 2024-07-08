#pragma once

#include <vector>
#include <string>
#include <map>

class IRenderLayerRef;

enum RenderDependencyFlags
{
    RENDERV_DEP_FIRST = (1 >> 0),
    RENDERV_DEP_LAST = (1 >> 1),
    RENDERV_DEP_BEFORE = (1 >> 2),
};

template <typename _DependencyT>
class RenderDependencyList
{
protected:
    std::map<_DependencyT*, std::vector<std::string>> DependencyList;
public:

    inline const std::map<_DependencyT*, std::vector<std::string>>& GetDependencyList() const { return DependencyList; }

    void AddDependency(_DependencyT* Source, std::string Dependency, bool Behind = false);

    void SetFlags(_DependencyT* Source, RenderDependencyFlags flags);

    void FlushSort(std::vector<_DependencyT*>* UnsortedArray);
};