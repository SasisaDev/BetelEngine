#pragma once
#include <vector>
#include <concepts>
#include <type_traits>

namespace BImGui
{
    struct DeferredDeleter {
        virtual bool Cleanup(){return false;}
    };

    extern std::vector<DeferredDeleter*> deferredDeleters;

    template<typename DerivedT>
    concept DeferredDeleterType = std::is_base_of<DeferredDeleter, DerivedT>::value;

    template <DeferredDeleterType DeleterType>
    extern DeleterType* CreateDeferredDeleter();

    extern void RemoveDeferredDeleter(DeferredDeleter* deleter);

    extern void FlushDeferredDeleters();

    extern void RemoveAllDeferredDeleters();
};

template <BImGui::DeferredDeleterType DeleterType>
inline DeleterType* BImGui::CreateDeferredDeleter()
{
    DeleterType* deleter = new DeleterType;
    deferredDeleters.push_back(deleter);
    return deleter;
}