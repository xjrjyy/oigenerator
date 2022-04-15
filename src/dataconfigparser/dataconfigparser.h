#ifndef OIGENERATOR_COMPILER_COMPILER_H_
#define OIGENERATOR_COMPILER_COMPILER_H_

#include <string>
#include <vector>

#include "filesystem.h"

class DataConfigParser {
public:
    DataConfigParser() = delete;
    DataConfigParser(const fs::path &);
    DataConfigParser(const DataConfigParser &) = default;
    DataConfigParser(DataConfigParser &&) = default;
    DataConfigParser &operator=(const DataConfigParser &) = default;
    DataConfigParser &operator=(DataConfigParser &&) = default;
    ~DataConfigParser() = default;
    bool Parse();
    std::vector<std::string> GetConfig() const;
private:
    fs::path path_;
    std::vector<std::string> config_;
};

#endif // OIGENERATOR_COMPILER_COMPILER_H_
