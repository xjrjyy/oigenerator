#ifndef OIGENERATOR_SRC_COMPILER_COMPILER_H
#define OIGENERATOR_SRC_COMPILER_COMPILER_H

#include <string>
#include <vector>

#include "filesystem.h"

class ConfigParser {
public:
    ConfigParser() = delete;
    ConfigParser(const fs::path &);
    ConfigParser(const ConfigParser &) = default;
    // TODO: ConfigParser &&
    ~ConfigParser() = default;
    bool Parse();
    std::vector<std::string> GetConfig() const;
private:
    fs::path path_;
    std::vector<std::string> config_;
};

#endif // OIGENERATOR_SRC_COMPILER_COMPILER_H
