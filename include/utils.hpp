#ifndef OIGENERATOR_UTILS_HPP_
#define OIGENERATOR_UTILS_HPP_

#include <string>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define OIGEN_WIN32
#define OIGEN_EXE_EXTENSION ".exe"
#elif __APPLE__
#define OIGEN_EXE_EXTENSION ".out"
#elif __linux__
#define OIGEN_EXE_EXTENSION ".out"
#elif __unix__
#define OIGEN_EXE_EXTENSION ".out"
#elif defined(_POSIX_VERSION)
#define OIGEN_EXE_EXTENSION ".out"
#else
#   error "Unknown compiler"
#endif

#endif // OIGENERATOR_UTILS_HPP_