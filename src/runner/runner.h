#ifndef OIGENERATOR_RUNNER_RUNNER_H_
#define OIGENERATOR_RUNNER_RUNNER_H_

#include <string>
#include <unordered_map>

#include "filesystem.h"

#include "filetype.h"
#include "config.h"

class Runner {
public:
    Runner() = delete;
    Runner(const fs::path &, const Config &);
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
    bool compiled_;
    Config config_;
};

#endif // OIGENERATOR_RUNNER_RUNNER_H_
