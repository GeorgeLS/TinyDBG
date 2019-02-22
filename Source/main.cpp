#include <iostream>
#include <memory>
#include <unistd.h>
#include "../Include/Utils.h"
#include "../Include/ProgramInfo.h"
#include "../Include/Debugger.h"

TinyDBG::ProgramInfo ParseArguments(size_t argc, char **argv) noexcept {
    TinyDBG::ProgramInfo program{};
    program.name = argv[1];
    program.arguments_n = argc - 2;
    program.arguments = std::make_unique<std::string[]>(argc - 2);
    for (size_t i = 2U; i < argc; ++i) {
        program.arguments[i - 2] = argv[i];
    }
    return program;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "You must provide more arguments!" << std::endl;
        exit(EXIT_FAILURE);
    }
    TinyDBG::ProgramInfo programInfo = ParseArguments(static_cast<size_t>(argc), argv);
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        TinyDBG::Utils::RegisterTrace();
        TinyDBG::Utils::ExecuteProgram(programInfo);
    } else if (pid > 0) {
        // parent process
        TinyDBG::Debugger dgb{programInfo, pid};
        dgb.Run();
    } else {
        std::cerr << "Error while creating the new process. Terminating..." << std::endl;
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}