#ifndef OIGENERATOR_SRC_COMPILER_COMPILER_H
#define OIGENERATOR_SRC_COMPILER_COMPILER_H

#include <string>
#include <vector>

#include "filesystem.h"

class DataConfigParser {
public:
    DataConfigParser() = delete;
    DataConfigParser(const fs::path &);
    DataConfigParser(const DataConfigParser &) = default;
    // TODO: DataConfigParser &&
    ~DataConfigParser() = default;
    bool Parse();
    std::vector<std::string> GetConfig() const;
private:
    fs::path path_;
    std::vector<std::string> config_;
};

#endif // OIGENERATOR_SRC_COMPILER_COMPILER_H
