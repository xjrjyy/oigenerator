#ifndef OIGENERATOR_SRC_COMPARER_COMPARER_H
#define OIGENERATOR_SRC_COMPARER_COMPARER_H

#include "filesystem.h"

class Comparer {
public:
    Comparer() {}
    Comparer(const Comparer &) = default;
    virtual ~Comparer() {};
    virtual bool Compare(const fs::path &, const fs::path &) const = 0;
    // TODO: Compare std::string & fs::ifstream
};

#endif // OIGENERATOR_SRC_COMPARER_COMPARER_H
