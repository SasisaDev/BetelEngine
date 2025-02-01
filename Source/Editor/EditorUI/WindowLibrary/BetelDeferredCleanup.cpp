#include "BetelDeferredCleanup.h"

namespace BImGui {
    std::vector<DeferredDeleter*> deferredDeleters;

    void RemoveDeferredDeleter(DeferredDeleter* deleter) {
        for(int i = 0; i < deferredDeleters.size(); ++i)
        {
            if(deferredDeleters[i] == deleter) {
                delete deleter;
                deferredDeleters.erase(deferredDeleters.begin() + i);
                return;
            }
        }
    }

    void FlushDeferredDeleters() {
        for(int i = 0; i < deferredDeleters.size(); ++i)
        {
            if(deferredDeleters[i] != nullptr)
            {
                if(deferredDeleters[i]->Cleanup())
                {
                    delete deferredDeleters[i];
                    deferredDeleters.erase(deferredDeleters.begin() + i);
                }
            } 
            else 
            {
                deferredDeleters.erase(deferredDeleters.begin() + i);
            }
        }
    }

    void RemoveAllDeferredDeleters() {
        for(; deferredDeleters.size() > 0;)
        {
            delete deferredDeleters[0];
            deferredDeleters.erase(deferredDeleters.begin());
        }
    } 
};