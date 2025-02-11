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

    extern void Begin(uint32_t object);

    extern void End(uint32_t object);

    extern uint32_t GetObject();
};
};