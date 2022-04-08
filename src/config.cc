#include "config.h"

Config::Config() {
    compiler_cpp_path_ = "g++";
    compile_cpp_command_ = "{compiler} \"{src}\" -std=c++14 -O2 -o \"{exe}\"";
    compiler_c_path_ = "gcc";
    compile_c_command_ = "{compiler} \"{src}\" -std=c11 -O2 -o \"{exe}\"";
    recompiling_ = false;
}

Config::Config(const std::map<std::string, std::string> &config) : Config() {
    if (config.count("compiler_cpp_path")) {
        compiler_cpp_path_ = config.at("compiler_cpp_path");
    }
    if (config.count("compile_cpp_command")) {
        compile_cpp_command_ = config.at("compile_cpp_command");
    }
    if (config.count("compiler_c_path")) {
        compiler_c_path_ = config.at("compiler_c_path");
    }
    if (config.count("compile_c_command")) {
        compile_c_command_ = config.at("compile_c_command");
    }
    if (config.count("recompiling")) {
        // TODO: string to bool
        recompiling_ = (config.at("recompiling") == "true");
    }
}

std::string Config::GetCompilerCppPath() const {
    return compiler_cpp_path_;
}
std::string Config::GetCompileCppCommand() const {
    return compile_cpp_command_;
}
std::string Config::GetCompilerCPath() const {
    return compiler_c_path_;
}
std::string Config::GetCompileCCommand() const {
    return compile_c_command_;
}
bool Config::GetRecompiling() const {
    return recompiling_;
}
