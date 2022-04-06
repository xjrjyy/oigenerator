#ifndef OIGENERATOR_CONFIG_H
#define OIGENERATOR_CONFIG_H

#include <string>

class Config {
public:
    static std::string compiler_cpp_path;
    static std::string compile_cpp_command;
    static std::string compiler_c_path;
    static std::string compile_c_command;
};

#endif // OIGENERATOR_CONFIG_H