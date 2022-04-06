#ifndef OIGENERATOR_SRC_RUNNER_RUNNER_H
#define OIGENERATOR_SRC_RUNNER_RUNNER_H

#include <string>
#include <unordered_map>

#include "filesystem.h"

#include "filetype.h"

class Runner {
public:
    Runner() = delete;
    Runner(const fs::path &, bool = false);
    Runner(const Runner &) = default;
    // TODO: Runner &&
    ~Runner() = default;
    int Start(
        const fs::path & = fs::path(),
        const fs::path & = fs::path(),
        const fs::path & = fs::path(),
        const std::string & = ""
    ); // TODO
private:
    static FileType FindFileType(const fs::path &);
    static const std::unordered_map<std::string, FileType> kExtFileType;
    fs::path path_;
    bool recompiling_, compiled_;
};

#endif // OIGENERATOR_SRC_RUNNER_RUNNER_H