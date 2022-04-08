#include "runner.h"

#include <cstdlib>
#include <iostream>

#include "fmt/core.h"

#include "compiler/compiler.h"

Runner::Runner(const fs::path &path, const Config &config)
    : path_(path), config_(config), compiled_(false) {}

int Runner::Start(
    const fs::path &input_path,
    const fs::path &output_path,
    const fs::path &error_path,
    const std::string &extra_command
) {
    FileType file_type = FindFileType(path_);
    if (file_type == FileType::Unknown) return -1; // TODO: Magic Number
    if (!fs::exists(path_)) return -1; // TODO: Magic Number

    std::string command = " " + extra_command;
    // TODO: Config
    if (!input_path.empty()) {
        command += fmt::format(" < \"{}\"", input_path.string());
    }
    if (!output_path.empty()) {
        command += fmt::format(" > \"{}\"", output_path.string());
    }
    if (!error_path.empty()) {
        command += fmt::format(" 2> \"{}\"", error_path.string());
    }

    int result = 0;
    if (file_type == FileType::Exe) { // TODO:
        result = system((path_.string() + command).c_str());
    } else if (file_type == FileType::C || file_type == FileType::Cpp) {
        fs::path exe_path = path_;
        exe_path.replace_extension(".exe");
        if (!compiled_ || config_.GetRecompiling()) {
            Compiler compiler(file_type, config_);
            result = compiler.Compile(path_, exe_path);
            if (result) return result; // TODO: Magic Number
            compiled_ = true;
        }
        result = system((exe_path.string() + command).c_str());
    } else if (file_type == FileType::Python) {
        // TODO: set python interpreter path
        result = system(("python " + path_.string() + command).c_str());
    } else {
        result = -1; // TODO: Magic Number
    }
    return 0;
}

FileType Runner::FindFileType(const fs::path &path) {
    auto type_iter = kExtFileType.find(path.extension().string());
    if (type_iter == kExtFileType.end()) return FileType::Unknown;
    return type_iter->second;
}

// TODO: config
const std::unordered_map<std::string, FileType> Runner::kExtFileType = {
    {".exe", FileType::Exe}, // Windows
    {".out", FileType::Exe}, // Linux
    {".c", FileType::C}, // TODO: all c/cpp extension
    {".cpp", FileType::Cpp},
    {".cc", FileType::Cpp},
    {".cxx", FileType::Cpp},
    {".py", FileType::Python},
};
