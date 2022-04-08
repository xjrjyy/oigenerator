#ifndef OIGENERATOR_CONFIG_H_
#define OIGENERATOR_CONFIG_H_

#include <string>
#include <map>
#include <chrono>

class Config {
public:
    Config();
    Config(const std::map<std::string, std::string> &);
    std::string GetCompilerCppPath() const;
    std::string GetCompileCppCommand() const;
    std::string GetCompilerCPath() const;
    std::string GetCompileCCommand() const;
    bool GetRecompiling() const;
    std::chrono::milliseconds GetTimeLimit() const;
    int GetMemoryLimit() const;
    bool GetPrintingUsed() const;
private:
    std::string compiler_cpp_path_;
    std::string compile_cpp_command_;
    std::string compiler_c_path_;
    std::string compile_c_command_;
    bool recompiling_;
    std::chrono::milliseconds time_limit_;
    int memory_limit_;
    bool printing_used_;
};

#endif // OIGENERATOR_CONFIG_H_