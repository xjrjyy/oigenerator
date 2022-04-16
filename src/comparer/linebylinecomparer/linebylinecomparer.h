#ifndef OIGENERATOR_COMPARER_LINEBYLINECOMPARER_H_
#define OIGENERATOR_COMPARER_LINEBYLINECOMPARER_H_

#include "comparer.h"

#include <string>

#include "filesystem.h"

class LineByLineComparer : public Comparer {
public:
    LineByLineComparer() = default;
    LineByLineComparer(const LineByLineComparer &) = default;
    LineByLineComparer(LineByLineComparer &&) = default;
    LineByLineComparer &operator=(const LineByLineComparer &) = default;
    LineByLineComparer &operator=(LineByLineComparer &&) = default;
    bool Compare(const fs::path &, const fs::path &) const override;
private:
    static void Trim(std::string &);
    static bool IsSpace(char);
};

#endif // OIGENERATOR_COMPARER_LINEBYLINECOMPARER_H_
