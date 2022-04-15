#ifndef OIGENERATOR_CONFIG_H_
#define OIGENERATOR_CONFIG_H_

#include <string>
#include <map>
#include <chrono>

class Config {
public:
    Config();
    Config(const std::map<std::string, std::string> &);
    Config(const Config &) = default;
    Config(Config &&) = default;
    Config &operator=(const Config &) = default;
    Config &operator=(Config &&) = default;
    std::string GetCompilerCppPath() const;
    std::string GetCompileCppCommand() const;
    std::string GetCompilerCPath() const;
    std::string GetCompileCCommand() const;
    std::string GetInterpreterPython() const;
    bool GetRecompiling() const;
    std::chrono::milliseconds GetTimeLimit() const;
    int GetMemoryLimit() const;
    bool GetPrintingUsed() const;
private:
    std::string compiler_cpp_path_;
    std::string compile_cpp_command_;
    std::string compiler_c_path_;
    std::string compile_c_command_;
    std::string interpreter_python_;
    bool recompiling_;
    std::chrono::milliseconds time_limit_;
    int memory_limit_;
    bool printing_used_;
};

#endif // OIGENERATOR_CONFIG_H_