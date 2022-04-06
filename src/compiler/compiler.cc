#include "compiler.h"

#include <iostream>

#include "fmt/core.h"

#include "config.h"

Compiler::Compiler(FileType file_type)
    : file_type_(file_type) {}

int Compiler::Compile(const fs::path &src_path, const fs::path &exe_path) {
    int result = 0;
    if (file_type_ == FileType::C) {
        std::string compile_command = fmt::format(
            Config::compile_c_command,
            fmt::arg("compiler", Config::compiler_c_path),
            fmt::arg("src", src_path.string()),
            fmt::arg("exe", exe_path.string())
        );
        result = system(compile_command.c_str());
    } else if (file_type_ == FileType::Cpp) {
        std::string compile_command = fmt::format(
            Config::compile_cpp_command,
            fmt::arg("compiler", Config::compiler_cpp_path),
            fmt::arg("src", src_path.string()),
            fmt::arg("exe", exe_path.string())
        );
        result = system(compile_command.c_str());
    } else {
        result = -1; // TODO: Magic Number
    }
    return result;
}
