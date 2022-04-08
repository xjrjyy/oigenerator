#include "compiler.h"

#include <iostream>

#include "fmt/core.h"

#include "config.h"

Compiler::Compiler(FileType file_type, const Config &config)
    : file_type_(file_type), config_(config) {}

int Compiler::Compile(const fs::path &src_path, const fs::path &exe_path) {
    int result = 0;
    if (file_type_ == FileType::C) {
        std::string compile_command = fmt::format(
            config_.GetCompileCCommand(),
            fmt::arg("compiler", config_.GetCompilerCPath()),
            fmt::arg("src", src_path.string()),
            fmt::arg("exe", exe_path.string())
        );
        result = system(compile_command.c_str());
    } else if (file_type_ == FileType::Cpp) {
        std::string compile_command = fmt::format(
            config_.GetCompileCppCommand(),
            fmt::arg("compiler", config_.GetCompilerCppPath()),
            fmt::arg("src", src_path.string()),
            fmt::arg("exe", exe_path.string())
        );
        result = system(compile_command.c_str());
    } else {
        result = -1; // TODO: Magic Number
    }
    return result;
}
