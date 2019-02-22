//
// Created by gliontos on 2/22/19.
//

#include <sys/ptrace.h>
#include <cerrno>
#include <iostream>
#include "../Include/Breakpoint.h"

TinyDBG::Breakpoint::Breakpoint(const pid_t pid, std::intptr_t address) : pid_{pid}, address{address}, enabled{false},
                                                                          saved_data_{} {}

void TinyDBG::Breakpoint::enable() {
    /*
     * The way this works is the following (in x86_64 architecture):
     * At the address specified there is some machine code in hexadecimal format
     * which will be executed when the cpu gets there. For example let's say that
     * in the address we provided we have the following code:
     *
     * (The left part is the hexadecimal code and the right part is the corresponding assembly code)
     * 48 89 E5 mov %rsp, %rbp
     * The 48 resembles the mov instruction. What we really need to do it's change this instruction
     * and replace it with the int 3 instruction without corrupting the data (89 E5).
     * We specifically need the int 3 instruction because when the cpu executes this instruction it will cause a trap
     * and call the Breakpoint Interrupt Handler which lies in the Interrupt Vector Table.
     * The operating system is attached to the Breakpoint Interrupt Handler and will send a SIGTRAP signal when
     * the trap occurs.
     */
    auto data = ptrace(PTRACE_PEEKDATA, pid_, address, nullptr);
    saved_data_ = static_cast<uint8_t>(data & 0xFF);
    uint64_t int3Instruction = 0xCC;
    uint64_t dataWithInt3 = ((data & ~0xFF) | int3Instruction);
    ptrace(PTRACE_POKEDATA, pid_, address, dataWithInt3);
    enabled = true;
}

void TinyDBG::Breakpoint::disable() {
    /*
     * When we are disabling the breakpoint we follow the above
     * logic and we just restore the previous instruction that
     * was in that address
     */
    auto data = ptrace(PTRACE_PEEKDATA, pid_, address, nullptr);
    auto restoredData = ((data & ~0xFF) | saved_data_);
    ptrace(PTRACE_POKEDATA, pid_, address, restoredData);
    enabled = false;
}