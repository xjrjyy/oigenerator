#ifndef OIGENERATOR_COMPARER_H_
#define OIGENERATOR_COMPARER_H_

#include "filesystem.h"

class Comparer {
public:
    Comparer() {}
    Comparer(const Comparer &) = default;
    virtual ~Comparer() {};
    virtual bool Compare(const fs::path &, const fs::path &) const = 0;
    // TODO: Compare std::string & fs::ifstream
};

#endif // OIGENERATOR_COMPARER_H_
