#ifndef OIGENERATOR_UTILS_HPP_
#define OIGENERATOR_UTILS_HPP_

#include <string>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define OIGEN_WIN32
std::string exe_extension = ".exe";
#elif __APPLE__
std::string exe_extension = ".out";
#elif __linux__
std::string exe_extension = ".out";
#elif __unix__
std::string exe_extension = ".out";
#elif defined(_POSIX_VERSION)
std::string exe_extension = ".out";
#else
#   error "Unknown compiler"
#endif

#endif // OIGENERATOR_UTILS_HPP_