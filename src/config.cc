#include "config.h"

std::string Config::compiler_cpp_path = "g++";
std::string Config::compile_cpp_command = "{compiler} \"{src}\" -std=c++14 -O2 -o \"{exe}\"";
std::string Config::compiler_c_path = "gcc";
std::string Config::compile_c_command = "{compiler} \"{src}\" -std=c11 -O2 -o \"{exe}\"";
