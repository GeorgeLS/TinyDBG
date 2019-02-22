//
// Created by gliontos on 2/22/19.
//

#ifndef DEBUGGER_UTILS_H
#define DEBUGGER_UTILS_H

#include <array>
#include <vector>
#include "ProgramInfo.h"

namespace TinyDBG::Utils {

    void RegisterTrace();

    void ExecuteProgram(const ProgramInfo &programInfo);

    std::string ReadLineFromStdin();

    std::vector<std::string> split(const std::string &string, char delimiter);

    bool is_prefix(const std::string &s, const std::string &of);
}

#endif //DEBUGGER_UTILS_H
