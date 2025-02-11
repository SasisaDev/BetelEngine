#include "BetelDrag.h"

namespace BImGui {
namespace Drag{
    void Begin(uint32_t object) {
        ::BImGui::Drag::Context::Context::Get().Object = object;
    }

    void End(uint32_t object) {
        if(::BImGui::Drag::Context::Context::Get().Object == object){
            ::BImGui::Drag::Context::Context::Get().Object = 0;
        }
    }

    uint32_t GetObject() {
        return ::BImGui::Drag::Context::Get().Object;
    }
};
};