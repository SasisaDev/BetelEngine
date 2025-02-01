#include "BetelDeferred.h"

namespace BImGui {
    std::vector<DeferredTask*> deferredTasks;

    void RemoveDeferredTask(DeferredTask* deleter) {
        for(int i = 0; i < deferredTasks.size(); ++i)
        {
            if(deferredTasks[i] == deleter) {
                delete deleter;
                deferredTasks.erase(deferredTasks.begin() + i);
                return;
            }
        }
    }

    void FlushDeferredTasks() {
        for(int i = 0; i < deferredTasks.size(); ++i)
        {
            if(deferredTasks[i] != nullptr)
            {
                if(deferredTasks[i]->Perform())
                {
                    delete deferredTasks[i];
                    deferredTasks.erase(deferredTasks.begin() + i);
                }
            } 
            else 
            {
                deferredTasks.erase(deferredTasks.begin() + i);
            }
        }
    }

    void RemoveAllDeferredTasks() {
        for(; deferredTasks.size() > 0;)
        {
            delete deferredTasks[0];
            deferredTasks.erase(deferredTasks.begin());
        }
    } 
};