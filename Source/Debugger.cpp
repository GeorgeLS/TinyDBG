//
// Created by gliontos on 2/22/19.
//

#include <sys/ptrace.h>
#include "../Include/Debugger.h"
#include "../Include/Utils.h"

TinyDBG::Debugger::Debugger(ProgramInfo &programInfo, const pid_t pid) : programInfo_{std::move(programInfo)},
                                                                         pid_{pid} {}

void TinyDBG::Debugger::Run() {
    int waitStatus;
    int options = 0;
    // Wait until the process is ready to be debugged (Until the SIGTRAP signal is sent)
    waitpid(pid_, &waitStatus, options);
    while (!exit_) {
        std::cout << "tinyDBG > ";
        std::string commandString = ReadLineFromStdin();
        if (commandString.empty()) continue;
        HandleCommand(commandString);
    }
}

void TinyDBG::Debugger::SetBreakpointAtAddress(std::intptr_t address) {
    Breakpoint breakpoint{pid_, address};
    breakpoint.enable();
    breakpoints_[address] = breakpoint;
    std::cout << "Set Breakpoint at address: 0x" << std::hex << address << std::endl;
}

void TinyDBG::Debugger::HandleCommand(const std::string &commandString) {
    auto tokens = split(commandString, ' ');
    std::string command = tokens[0];
    if (is_prefix(command, "continue")) {
        ContinueExecution();
    } else if (is_prefix(command, "break")) {
        // No checks for now. Assume the user has written 0xAdrress
        std::string address{tokens[1], 2};
        SetBreakpointAtAddress(std::stol(address, nullptr, 16));
    } else if (is_prefix(command, "exit")) {
        exit_ = true;
    } else {
        std::cerr << "Unknown command!" << std::endl;
    }
}

void TinyDBG::Debugger::ContinueExecution() {
    ptrace(PTRACE_CONT, pid_, nullptr, nullptr);
    int waitStatus;
    int options = 0;
    waitpid(pid_, &waitStatus, options);
}