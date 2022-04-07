#ifndef OIGENERATOR_SRC_COMPARER_LINEBYLINECOMPARER_H
#define OIGENERATOR_SRC_COMPARER_LINEBYLINECOMPARER_H

#include "comparer.h"

#include <string>

#include "filesystem.h"

class LineByLineComparer : public Comparer {
public:
    LineByLineComparer() = default;
    LineByLineComparer(const LineByLineComparer &) = default;
    bool Compare(const fs::path &, const fs::path &) const override;
private:
    static void Trim(std::string &);
    static bool IsSpace(char);
};

#endif // OIGENERATOR_SRC_COMPARER_LINEBYLINECOMPARER_H
