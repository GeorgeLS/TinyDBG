//
// Created by gliontos on 2/22/19.
//

#include <sys/ptrace.h>
#include "../Include/Breakpoint.h"

TinyDBG::Breakpoint::Breakpoint(const pid_t pid, std::intptr_t address) : pid_{pid}, address{address}, enabled{false},
                                                                          saved_data_{} {}

void TinyDBG::Breakpoint::enable() {
    auto data = ptrace(PTRACE_PEEKDATA, pid_, address, nullptr);
    saved_data_ = static_cast<uint8_t>(data & 0xFF);
    uint64_t int3Instruction = 0xCC;
    uint64_t dataWithInt3 = ((data & ~0xFF) | int3Instruction);
    ptrace(PTRACE_POKEDATA, pid_, address, dataWithInt3);
    enabled = true;
}

void TinyDBG::Breakpoint::disable() {
    auto data = ptrace(PTRACE_PEEKDATA, pid_, address, nullptr);
    auto restoredData = ((data & ~0xFF) | saved_data_);
    ptrace(PTRACE_POKEDATA, pid_, address, restoredData);
    enabled = false;
}