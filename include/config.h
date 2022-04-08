#ifndef OIGENERATOR_CONFIG_H
#define OIGENERATOR_CONFIG_H

#include <string>
#include <map>

class Config {
public:
    Config();
    Config(const std::map<std::string, std::string> &);
    std::string GetCompilerCppPath() const;
    std::string GetCompileCppCommand() const;
    std::string GetCompilerCPath() const;
    std::string GetCompileCCommand() const;
    bool GetRecompiling() const;
private:
    std::string compiler_cpp_path_;
    std::string compile_cpp_command_;
    std::string compiler_c_path_;
    std::string compile_c_command_;
    bool recompiling_;
};

#endif // OIGENERATOR_CONFIG_H