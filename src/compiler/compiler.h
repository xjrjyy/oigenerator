#ifndef OIGENERATOR_SRC_COMPILER_COMPILER_H
#define OIGENERATOR_SRC_COMPILER_COMPILER_H

#include "filesystem.h"

#include "filetype.h"

class Compiler {
public:
    Compiler() = delete;
    Compiler(FileType);
    Compiler(const Compiler &) = default;
    // TODO: Compiler &&
    ~Compiler() = default;
    int Compile(const fs::path &, const fs::path &); // TODO
private:
    FileType file_type_;
};

#endif // OIGENERATOR_SRC_COMPILER_COMPILER_H
