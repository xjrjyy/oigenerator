#ifndef OIGENERATOR_COMPILER_COMPILER_H_
#define OIGENERATOR_COMPILER_COMPILER_H_

#include "filesystem.h"

#include "filetype.h"
#include "config.h"

class Compiler {
public:
    Compiler() = delete;
    Compiler(FileType, const Config &);
    Compiler(const Compiler &) = default;
    Compiler(Compiler &&) = default;
    Compiler &operator=(const Compiler &) = default;
    Compiler &operator=(Compiler &&) = default;
    ~Compiler() = default;
    int Compile(const fs::path &, const fs::path &); // TODO
private:
    FileType file_type_;
    Config config_;
};

#endif // OIGENERATOR_COMPILER_COMPILER_H_
