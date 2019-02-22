//
// Created by gliontos on 2/22/19.
//

#include <sys/ptrace.h>
#include <sys/user.h>
#include <iostream>
#include "../Include/RegisterUtils.h"

auto inline FindRegisterDescriptor(TinyDBG::Utils::RegisterUtils::reg reg) {
    return std::find_if(TinyDBG::Utils::RegisterUtils::GlobalRegistrerDescriptors.begin(),
                        TinyDBG::Utils::RegisterUtils::GlobalRegistrerDescriptors.end(),
                        [reg](auto &&rd) { return rd.r == reg; });
}

auto inline FindRegisterDescriptor(int dwarfReg) {
    return std::find_if(TinyDBG::Utils::RegisterUtils::GlobalRegistrerDescriptors.begin(),
                        TinyDBG::Utils::RegisterUtils::GlobalRegistrerDescriptors.end(),
                        [dwarfReg](auto &&rd) { return rd.dwarfRegister == dwarfReg; });
}

auto inline FindRegisterDescriptor(const std::string &name) {
    return std::find_if(TinyDBG::Utils::RegisterUtils::GlobalRegistrerDescriptors.begin(),
                        TinyDBG::Utils::RegisterUtils::GlobalRegistrerDescriptors.end(),
                        [name](auto &&rd) { return rd.name == name; });
}

uint64_t TinyDBG::Utils::RegisterUtils::ReadRegisterValue(pid_t pid, reg reg) {
    user_regs_struct regs{};
    ptrace(PTRACE_GETREGS, pid, nullptr, &regs);
    // NOTE(gliontos): We could use a big switch statement but to minimize code we are going to use some c++ magic
    auto iterator = FindRegisterDescriptor(reg);
    /* Now our iterator acts as the index of our register value inside the Global Register Descriptor table.
     * We can treat the user_regs_struct as an array of uint64_ts because of it's layout. Then add to the base address
     * the offset to get the right register
     */
    return *(reinterpret_cast<uint64_t *>(&regs) + (iterator - GlobalRegistrerDescriptors.begin()));
}

void SetRegisterValue(pid_t pid, TinyDBG::Utils::RegisterUtils::reg reg, uint64_t value) {
    user_regs_struct regs{};
    ptrace(PTRACE_GETREGS, pid, nullptr, &regs);
    // Same magic as above
    auto iterator = FindRegisterDescriptor(reg);
    *(reinterpret_cast<uint64_t *>(&regs) +
      (iterator - TinyDBG::Utils::RegisterUtils::GlobalRegistrerDescriptors.begin())) = value;
    ptrace(PTRACE_SETREGS, pid, nullptr, &regs);
}

bool ReadRegisterValueFromDWARF(pid_t pid, int dwarfReg, uint64_t *value_out) {
    auto iterator = FindRegisterDescriptor(dwarfReg);
    if (iterator == TinyDBG::Utils::RegisterUtils::GlobalRegistrerDescriptors.end()) {
        std::cerr << "Unknown dwarf register" << std::endl;
        return false;
    }
    *value_out = TinyDBG::Utils::RegisterUtils::ReadRegisterValue(pid, iterator->r);
    return true;
}

std::string ReadRegisterName(TinyDBG::Utils::RegisterUtils::reg reg) {
    auto iterator = FindRegisterDescriptor(reg);
    return iterator->name;
}

TinyDBG::Utils::RegisterUtils::reg GetRegisterFromName(const std::string &name) {
    auto iterator = FindRegisterDescriptor(name);
    return iterator->r;
}