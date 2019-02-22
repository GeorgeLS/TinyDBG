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
#include "Utils.h"

namespace TinyDBG {
    struct Debugger {
        Debugger(ProgramInfo &programInfo, pid_t pid);

        ~Debugger() = default;

        void Run();

    private:
        void HandleCommand(const std::string &commandString);

        void ContinueExecution();

        void SetBreakpointAtAddress(std::intptr_t address);

        void DumpRegisters();

        // For now just read and write 1 word at a time
        uint64_t inline ReadMemory(uint64_t address);

        void inline WriteMemory(uint64_t address, uint64_t value);

        void StepOverBreakpoint();

        uint64_t inline ReadPC();

        void inline SetPC(uint64_t pc);

        void inline WaitForSignal();

        ProgramInfo programInfo_;
        pid_t pid_;
        bool exit_{false};
        // NOTE(gliontos): Use STL for now. Maybe we implement our own hash table in the future
        std::unordered_map<std::intptr_t, Breakpoint> breakpoints_;
    };
}

#endif //DEBUGGER_DEBUGGER_H
