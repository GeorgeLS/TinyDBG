//
// Created by gliontos on 2/22/19.
//

#ifndef DEBUGGER_REGISTERUTILS_H
#define DEBUGGER_REGISTERUTILS_H

#include <string>
#include <array>
#include <algorithm>

namespace TinyDBG::Utils::RegisterUtils {
    // We will leave out floating points and SSE/AVX registers for now and for simplicity
    enum reg {
        rax, rbx, rcx, rdx,
        rdi, rsi, rbp, rsp,
        r8, r9, r10, r11,
        r12, r13, r14, r15,
        rip, rflags, cs,
        orig_rax, fs_base,
        gs_base,
        fs, gs, ss, ds, es
    };

    constexpr std::size_t n_registers = 27U;

    struct RegisterDescriptor {
        reg r;
        int dwarfRegister;
        std::string name;
    };

    // NOTE(gliontos): We layout our values the same way as user_regs_struct in order to search in a convenient manner
    const std::array<RegisterDescriptor, n_registers> GlobalRegistrerDescriptors{
            {
                    {reg::r15, 15, "r15"},
                    {reg::r14, 14, "r14"},
                    {reg::r13, 13, "r13"},
                    {reg::r12, 12, "r12"},
                    {reg::rbp, 6, "rbp"},
                    {reg::rbx, 3, "rbx"},
                    {reg::r11, 11, "r11"},
                    {reg::r10, 10, "r10"},
                    {reg::r9, 9, "r9"},
                    {reg::r8, 8, "r8"},
                    {reg::rax, 0, "rax"},
                    {reg::rcx, 2, "rcx"},
                    {reg::rdx, 1, "rdx"},
                    {reg::rsi, 4, "rsi"},
                    {reg::rdi, 5, "rdi"},
                    {reg::orig_rax, -1, "orig_rax"},
                    {reg::rip, -1, "rip"},
                    {reg::cs, 51, "cs"},
                    {reg::rflags, 49, "eflags"},
                    {reg::rsp, 7, "rsp"},
                    {reg::ss, 52, "ss"},
                    {reg::fs_base, 58, "fs_base"},
                    {reg::gs_base, 59, "gs_base"},
                    {reg::ds, 53, "ds"},
                    {reg::es, 50, "es"},
                    {reg::fs, 54, "fs"},
                    {reg::gs, 55, "gs"},
            }
    };

    uint64_t ReadRegisterValue(pid_t pid, reg reg);

    void SetRegisterValue(pid_t pid, reg reg, uint64_t value);

    bool ReadRegisterValueFromDWARF(pid_t pid, int dwarfReg, uint64_t *value_out);

    std::string ReadRegisterName(reg reg);

    reg GetRegisterFromName(const std::string &name);
}

#endif //DEBUGGER_REGISTERUTILS_H
