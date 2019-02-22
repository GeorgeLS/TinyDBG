//
// Created by gliontos on 2/22/19.
//

#include <sys/ptrace.h>
#include <sys/user.h>
#include <iomanip>
#include "../Include/Debugger.h"
#include "../Include/Utils.h"
#include "../Include/RegisterUtils.h"

using namespace TinyDBG::Utils;
using namespace TinyDBG::Utils::RegisterUtils;

TinyDBG::Debugger::Debugger(ProgramInfo &programInfo, const pid_t pid) : programInfo_{std::move(programInfo)},
                                                                         pid_{pid} {}

void TinyDBG::Debugger::Run() {
    WaitForSignal();
    while (!exit_) {
        std::cout << "tinyDBG > ";
        std::string commandString = ReadLineFromStdin();
        if (commandString.empty()) continue;
        HandleCommand(commandString);
    }
}

void TinyDBG::Debugger::HandleCommand(const std::string &commandString) {
    auto tokens = split(commandString, ' ');
    std::string command = tokens[0];
    if (is_prefix(command, "continue")) {
        ContinueExecution();
    } else if (is_prefix(command, "break")) {
        // No checks for now. Assume the user has written 0xAdrress
        std::string address{tokens[1], 3};
        SetBreakpointAtAddress(std::stol(address, nullptr, 16));
    } else if (is_prefix(command, "register")) {
        if (is_prefix(tokens[1], "dump")) {
            DumpRegisters();
        } else if (is_prefix(tokens[1], "read")) {
            std::cout << ReadRegisterValue(pid_, GetRegisterFromName(tokens[2])) << std::endl;
        } else if (is_prefix(tokens[1], "write")) {
            reg reg = GetRegisterFromName(tokens[2]);
            std::string value{tokens[3], 3}; // Assume hexadecimal value e.g 0xFF0
            SetRegisterValue(pid_, reg, static_cast<uint64_t>(std::stol(value, nullptr, 16)));
        }
    } else if (is_prefix(command, "memory")) {
        std::string _address{tokens[2], 3};
        uint64_t address = std::stol(_address, nullptr, 16);
        if (is_prefix(tokens[1], "read")) {
            std::cout << std::hex << ReadMemory(address) << std::endl;
        } else if (is_prefix(tokens[1], "write")) {
            std::string value{tokens[3], 3};
            WriteMemory(address, std::stol(value, nullptr, 16));
        }
    } else if (is_prefix(command, "exit") or is_prefix(command, "quit")) {
        exit_ = true;
    } else {
        std::cerr << "Unknown command!" << std::endl;
    }
}

void TinyDBG::Debugger::ContinueExecution() {
    StepOverBreakpoint();
    ptrace(PTRACE_CONT, pid_, nullptr, nullptr);
    WaitForSignal();
}

void TinyDBG::Debugger::SetBreakpointAtAddress(std::intptr_t address) {
    Breakpoint breakpoint{pid_, address};
    breakpoint.enable();
    breakpoints_[address] = breakpoint;
    std::cout << "Set Breakpoint at address: 0x" << std::hex << address << std::endl;
}

void TinyDBG::Debugger::DumpRegisters() {
    for (const auto &rd : GlobalRegistrerDescriptors) {
        std::cout << rd.name << " 0x" << std::setfill('0')
                  << std::setw(16) << std::hex
                  << ReadRegisterValue(pid_, rd.r)
                  << std::endl;
    }
}

uint64_t TinyDBG::Debugger::ReadMemory(uint64_t address) {
    return static_cast<uint64_t>(ptrace(PTRACE_PEEKDATA, pid_, address, nullptr));
}

void TinyDBG::Debugger::WriteMemory(uint64_t address, uint64_t value) {
    ptrace(PTRACE_POKEDATA, pid_, address, value);
}

uint64_t inline TinyDBG::Debugger::ReadPC() {
    return ReadRegisterValue(pid_, reg::rip);
}

void inline TinyDBG::Debugger::SetPC(uint64_t pc) {
    SetRegisterValue(pid_, reg::rip, pc);
}

void inline TinyDBG::Debugger::WaitForSignal() {
    int waitStatus;
    int options = 0;
    waitpid(pid_, &waitStatus, options);
}

void TinyDBG::Debugger::StepOverBreakpoint() {
    /*
     * NOTE(gliontos): We subtract 1 because the cpu will have executed the instruction
     * at our breakpoint. Remember that this instruction is int 3 which is 1 byte long
     */
    uint64_t possibleBreakpointLocation = ReadPC() - 1;
    if (breakpoints_.count(possibleBreakpointLocation)) {
        Breakpoint &breakpoint = breakpoints_[possibleBreakpointLocation];
        if (breakpoint.enabled) {
            uint64_t previousInstructionAddress = possibleBreakpointLocation;
            SetPC(previousInstructionAddress);
            breakpoint.disable();
            ptrace(PTRACE_SINGLESTEP, pid_, nullptr, nullptr);
            WaitForSignal();
            breakpoint.enable();
        }
    }
}