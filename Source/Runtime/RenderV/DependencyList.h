#pragma once

#include <vector>
#include <string>
#include <map>

class IRenderLayerRef;

/*
 * Flags defining Dependency Injection behaviour
 * TOP_OF_PILE: inject object as soon as possible in the pile
 * BOTTOM_OF_PILE: inject object as late as possible in the pile
 * BEFORE: inject object right before dependency
 * AFTER: inject object right after dependency
*/
enum RenderDependencyFlags
{
    RENDERV_DEP_BOTTOM_OF_PILE = (1 << 0),
    RENDERV_DEP_TOP_OF_PILE = (1 << 1),
    RENDERV_DEP_BEFORE = (1 << 2),
    RENDERV_DEP_AFTER = (1 << 3),
};

template <typename _DependencyT>
class RenderDependencyList
{
    struct DependencyData   
    {
        std::string name;
        uint8_t flags;
        DependencyData(std::string n, uint8_t f):name(n), flags(f) {}
    };
protected:
    std::map<_DependencyT*, std::vector<DependencyData>> DependencyList;
public:

    inline const std::map<_DependencyT*, std::vector<DependencyData>>& GetDependencyList() const { return DependencyList; }

    /*
     * Inject Source according to Dependency
     * With default settings, Source will be injected as soon as possible in the pile
     * Right after the last dependency
     */ 
    void AddDependency(_DependencyT* Source, std::string Dependency, uint8_t Flags = RENDERV_DEP_BOTTOM_OF_PILE) {
        if(DependencyList.contains(Source)) {
            DependencyList[Source].push_back(DependencyData(Dependency, Flags));
        } else {
            DependencyList.insert_or_assign(Source, std::vector<DependencyData>{DependencyData(Dependency, Flags)});
        }
    }

    void SetFlags(_DependencyT* Source, RenderDependencyFlags flags);

    void FlushSort(std::vector<_DependencyT*>* UnsortedArray);
};