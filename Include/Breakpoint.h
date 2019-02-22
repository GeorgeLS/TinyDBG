//
// Created by gliontos on 2/22/19.
//

#ifndef DEBUGGER_BREAKPOINT_H
#define DEBUGGER_BREAKPOINT_H

#include <unistd.h>
#include <cstdint>

namespace TinyDBG {
    struct Breakpoint {
        Breakpoint() = default;

        Breakpoint(pid_t pid, std::intptr_t address);

        ~Breakpoint() = default;

        void enable();

        void disable();

        std::intptr_t address;
        bool enabled;
    private:
        pid_t pid_;
        uint8_t saved_data_;
    };
}

#endif //DEBUGGER_BREAKPOINT_H
