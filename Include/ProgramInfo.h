//
// Created by gliontos on 2/22/19.
//

#ifndef DEBUGGER_PROGRAMINFO_H
#define DEBUGGER_PROGRAMINFO_H

#include <memory>
#include <string>

namespace TinyDBG {
    struct ProgramInfo {
        ProgramInfo() = default;

        ProgramInfo(ProgramInfo &&rValue) noexcept;

        ~ProgramInfo() = default;

        std::string name;
        std::unique_ptr<std::string[]> arguments;
        size_t arguments_n{};
    };
}

#endif //DEBUGGER_PROGRAMINFO_H
