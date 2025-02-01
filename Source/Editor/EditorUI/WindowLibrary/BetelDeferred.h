#pragma once
#include <vector>
#include <concepts>
#include <type_traits>

namespace BImGui
{
    struct DeferredTask {
        virtual bool Perform(){return false;}
    };

    extern std::vector<DeferredTask*> deferredTasks;

    template<typename DerivedT>
    concept DeferredTaskType = std::is_base_of<DeferredTask, DerivedT>::value;

    template <DeferredTaskType TaskType>
    extern TaskType* CreateDeferredTask();

    extern void RemoveDeferredTask(DeferredTask* deleter);

    extern void FlushDeferredTasks();

    extern void RemoveAllDeferredTasks();
};

template <BImGui::DeferredTaskType TaskType>
inline TaskType* BImGui::CreateDeferredTask()
{
    TaskType* deleter = new TaskType;
    deferredTasks.push_back(deleter);
    return deleter;
}