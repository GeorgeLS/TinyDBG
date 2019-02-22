//
// Created by gliontos on 2/22/19.
//

#ifndef DEBUGGER_DEBUGGER_H
#define DEBUGGER_DEBUGGER_H

#include <unistd.h>
#include <algorithm>
#include <wait.h>
#include <iostream>
#include <unordered_map>
#include "Breakpoint.h"
#include "ProgramInfo.h"

namespace TinyDBG {
    struct Debugger {
        Debugger(ProgramInfo &programInfo, pid_t pid);

        ~Debugger() = default;

        void Run();

        void SetBreakpointAtAddress(std::intptr_t address);

    private:
        void HandleCommand(const std::string &commandString);

        void ContinueExecution();

        ProgramInfo programInfo_;
        pid_t pid_;
        bool exit_{false};
        // NOTE(gliontos): Use STL for now. Maybe we implement our own hash table in the future
        std::unordered_map<std::intptr_t, Breakpoint> breakpoints_;
    };
}

#endif //DEBUGGER_DEBUGGER_H
