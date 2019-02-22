//
// Created by gliontos on 2/22/19.
//

#include <sys/ptrace.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include "../Include/Utils.h"

void TinyDBG::RegisterTrace() { ptrace(PTRACE_TRACEME, 0, nullptr, nullptr); }

void TinyDBG::ExecuteProgram(const TinyDBG::ProgramInfo &programInfo) {
    char *argumentsAsCStrings[programInfo.arguments_n + 2];
    char *programName = const_cast<char *>(programInfo.name.c_str());
    argumentsAsCStrings[0] = programName;
    for (size_t i = 1U; i < programInfo.arguments_n; ++i) {
        argumentsAsCStrings[i] = const_cast<char *>(programInfo.arguments[i].c_str());
    }
    argumentsAsCStrings[programInfo.arguments_n + 1] = NULL;
    execv(programName, argumentsAsCStrings);
}

std::string TinyDBG::ReadLineFromStdin() {
    char input[64] = {0};
    for (size_t i = 0U; i != 64U and !std::cin.eof(); ++i) {
        std::cin >> std::noskipws >> input[i];
        if (input[i] == '\n') break;
    }
    size_t i = 0U;
    while (input[i] != '\n' and input[i] != EOF) { ++i; }
    input[i] = '\0';
    return input;
}

std::vector<std::string> TinyDBG::split(const std::string &string, const char delimiter) {
    std::vector<std::string> res{};
    char *_c_str = const_cast<char *>(string.c_str());
    size_t i = 0;
    size_t oldPosition = i;
    while (_c_str[i]) {
        if (_c_str[i] == delimiter) {
            char old = _c_str[i];
            _c_str[i] = '\0';
            std::string token(_c_str + oldPosition);
            res.emplace_back(token);
            _c_str[i] = old;
            oldPosition = i;
        }
        ++i;
    }
    res.emplace_back(_c_str + oldPosition);
    return res;
}

bool TinyDBG::is_prefix(const std::string &s, const std::string &of) {
    if (s.size() > of.size()) return false;
    // TODO(gliontos): Optimize this without copying the strings
    std::string _s{s};
    std::string _of{of};
    std::transform(_s.begin(), _s.end(), _s.begin(), ::tolower);
    std::transform(_of.begin(), _of.end(), _of.begin(), ::tolower);
    return std::equal(_s.begin(), _s.end(), _of.begin());
}