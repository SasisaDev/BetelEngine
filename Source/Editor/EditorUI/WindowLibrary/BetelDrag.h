#pragma once
#include <imgui/imgui_internal.h>
#include <cstdint>

namespace BImGui {
namespace Drag{
    class Context {
    public:
        uint32_t Object = 0;

        static Context& Get(){
            static Context ctx;
            return ctx;
        }
    };

    void Begin(uint32_t object) {
        Context::Get().Object = object;
    }

    void End(uint32_t object) {
        Context::Get().Object = 0;
    }

    uint32_t GetObject() {
        return Context::Get().Object;
    }
};
};