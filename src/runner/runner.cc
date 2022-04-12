#include "runner.h"

#include <cstdlib>
#include <iostream>
#include <thread>

#include "fmt/core.h"
#include "fmt/xchar.h"

#include "utils.hpp"
#include "compiler/compiler.h"

#if defined(OIGEN_WIN32)
#include <windows.h>
#include <Psapi.h>
#undef max
#undef min
#endif

Runner::Runner(const fs::path &path, const Config &config)
    : path_(path), config_(config), compiled_(false) {}

// TODO: Return message
int Runner::Start(
    const fs::path &input_path,
    const fs::path &output_path,
    const fs::path &error_path,
    const std::string &extra_arguments
) {
    FileType file_type = FindFileType(path_);
    if (file_type == FileType::Unknown) return -1; // TODO: Magic Number
    if (!fs::exists(path_)) return -1; // TODO: Magic Number

    int result = 0;
    // TODO: time used
    // TODO: extra time
#if defined(OIGEN_WIN32)
    std::wstring arguments(extra_arguments.begin(), extra_arguments.end());
    std::wstring exe_file;
    if (file_type == FileType::Exe) {
        exe_file = path_.wstring();
    } else if (file_type == FileType::C || file_type == FileType::Cpp) {
        fs::path exe_path = path_;
        exe_path.replace_extension(OIGEN_EXE_EXTENSION);
        if (!compiled_ || config_.GetRecompiling()) {
            Compiler compiler(file_type, config_);
            result = compiler.Compile(path_, exe_path);
            if (result) return result; // TODO: Magic Number
            compiled_ = true;
        }
        exe_file = exe_path.wstring();
    } else if (file_type == FileType::Python) {
        std::string exe_file_string = config_.GetInterpreterPython();
        exe_file = std::wstring(exe_file_string.begin(), exe_file_string.end()); // TODO: Env
        arguments = fmt::format(L"{} {}", path_.wstring(), arguments);
    } else {
        return -1; // TODO: Magic Number
    }
    SetErrorMode(SEM_NOGPFAULTERRORBOX);
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&sa, sizeof(sa));
    sa.bInheritHandle = TRUE;
    if (!input_path.empty()) {
        si.hStdInput = CreateFileW((const WCHAR *)(input_path.wstring().c_str()),
                                    GENERIC_READ,
                                    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                    &sa, OPEN_ALWAYS,
                                    FILE_ATTRIBUTE_NORMAL, NULL);
        if (si.hStdInput == INVALID_HANDLE_VALUE) {
            std::cout << "Unable to open input file" << std::endl;
        }
    }
    if (!output_path.empty()) {
        si.hStdOutput = CreateFileW((const WCHAR *)(output_path.wstring().c_str()),
                                    GENERIC_WRITE,
                                    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                    &sa, CREATE_ALWAYS,
                                    FILE_ATTRIBUTE_NORMAL, NULL);
        if (si.hStdOutput == INVALID_HANDLE_VALUE) {
            std::cout << "Unable to open output file" << std::endl;
        }
    }
    if (!error_path.empty()) {
        si.hStdError = CreateFileW((const WCHAR *)(error_path.wstring().c_str()),
                                GENERIC_WRITE,
                                FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                &sa, CREATE_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL, NULL);
        if (si.hStdError == INVALID_HANDLE_VALUE) {
            std::cout << "Unable to open error file" << std::endl;
        }
    }

    // TODO: env, working directory
    PTSTR pEnvBlock = GetEnvironmentStrings();
    if (!CreateProcessW(NULL,
                         (WCHAR *)(fmt::format(L"\"{}\" {}", exe_file, arguments).c_str()),
                         NULL,
                         &sa, TRUE, HIGH_PRIORITY_CLASS | CREATE_NO_WINDOW,
                         pEnvBlock,
                         NULL, // (const WCHAR *)(workingDirectory),
                         &si, &pi)) {
        if (!input_path.empty()) CloseHandle(si.hStdInput);
        if (!output_path.empty()) CloseHandle(si.hStdOutput);
        if (!error_path.empty()) CloseHandle(si.hStdError);
        FreeEnvironmentStrings(pEnvBlock);
        std::cout << "Unable to create process!" << std::endl;
        return -1; // TODO: Magic Number
    }
    FreeEnvironmentStrings(pEnvBlock);

    PROCESS_MEMORY_COUNTERS_EX info;
    ZeroMemory(&info, sizeof(info));
    info.cb = sizeof(info);
    if (config_.GetMemoryLimit() != -1) {
        GetProcessMemoryInfo(pi.hProcess, (PROCESS_MEMORY_COUNTERS *)&info, sizeof(info));

        if (std::max(info.PrivateUsage, info.PeakWorkingSetSize) > (unsigned int)config_.GetMemoryLimit() * 1024 * 1024) {
            TerminateProcess(pi.hProcess, 0);

            if (!input_path.empty()) CloseHandle(si.hStdInput);
            if (!output_path.empty()) CloseHandle(si.hStdOutput);
            if (!error_path.empty()) CloseHandle(si.hStdError);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            std::cout << "Memory Limit Exceeded" << std::endl;
            return -1; // TODO: Magic Number
        }
    }
    bool successful_finished = false;
    
    auto start_time = std::chrono::steady_clock::now(), end_time = start_time;
    while (end_time - start_time <= config_.GetTimeLimit()) {
        if (WaitForSingleObject(pi.hProcess, 0) == WAIT_OBJECT_0) {
            successful_finished = true;
            break;
        }

        if (config_.GetMemoryLimit() != -1) {
            GetProcessMemoryInfo(pi.hProcess, (PROCESS_MEMORY_COUNTERS *)&info, sizeof(info));

            if (std::max(info.PrivateUsage, info.PeakWorkingSetSize) > (unsigned int)config_.GetMemoryLimit() * 1024U * 1024) {
                TerminateProcess(pi.hProcess, 0);

                if (!input_path.empty()) CloseHandle(si.hStdInput);
                if (!output_path.empty()) CloseHandle(si.hStdOutput);
                if (!error_path.empty()) CloseHandle(si.hStdError);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                std::cout << "Memory Limit Exceeded" << std::endl;
                return -1; // TODO: Magic Number
            }
        }
        bool stop = false; // TODO: stop
        if (stop) {
            TerminateProcess(pi.hProcess, 0);

            if (!input_path.empty()) CloseHandle(si.hStdInput);
            if (!output_path.empty()) CloseHandle(si.hStdOutput);
            if (!error_path.empty()) CloseHandle(si.hStdError);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            std::cout << "Stop Force" << std::endl;
            return -1; // TODO: Magic Number
        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(10ms); // TODO: config
        end_time = std::chrono::steady_clock::now();
    }
    if (!successful_finished) {
        TerminateProcess(pi.hProcess, 0);

        if (!input_path.empty()) CloseHandle(si.hStdInput);
        if (!output_path.empty()) CloseHandle(si.hStdOutput);
        if (!error_path.empty()) CloseHandle(si.hStdError);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        std::cout << "Time Limit Exceeded" << std::endl;
        return -1; // TODO: Magic Number
    }

    unsigned long exit_code;
    GetExitCodeProcess(pi.hProcess, &exit_code);

    if (exit_code != 0) {
        if (!input_path.empty()) CloseHandle(si.hStdInput);
        if (!output_path.empty()) CloseHandle(si.hStdOutput);
        if (!error_path.empty()) CloseHandle(si.hStdError);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        std::cout << fmt::format("exit code is {}\n", exit_code) << std::endl;
        return -1; // TODO: Magic Number
    }

    FILETIME creationTime, exitTime, kernelTime, userTime;
    GetProcessTimes(pi.hProcess, &creationTime, &exitTime, &kernelTime, &userTime);
    SYSTEMTIME realUserTime, realKernelTime;
    FileTimeToSystemTime(&userTime, &realUserTime);
    FileTimeToSystemTime(&kernelTime, &realKernelTime);
    std::size_t time_used = realUserTime.wMilliseconds + realUserTime.wSecond * 1000 + realUserTime.wMinute * 60 * 1000 +
               realUserTime.wHour * 60 * 60 * 1000;
    int kernel_time_used = realKernelTime.wMilliseconds + realKernelTime.wSecond * 1000 +
                         realKernelTime.wMinute * 60 * 1000 + realKernelTime.wHour * 60 * 60 * 1000;
    GetProcessMemoryInfo(pi.hProcess, (PROCESS_MEMORY_COUNTERS *)&info, sizeof(info));
    std::size_t memory_used = info.PeakWorkingSetSize;

    if (!input_path.empty()) CloseHandle(si.hStdInput);
    if (!output_path.empty()) CloseHandle(si.hStdOutput);
    if (!error_path.empty()) CloseHandle(si.hStdError);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if (config_.GetPrintingUsed()) {
        std::cout << fmt::format(
            "use {}ms and {}mb",
            time_used,
            memory_used / 1024.0 / 1024.0
        ) << std::endl;
    }
#elif defined(OIGEN_LINUX)
    // TODO: Replace " to \"
    std::string arguments = extra_arguments;
    // TODO: Config
    fs::path exe_path;
    if (file_type == FileType::Exe) { // TODO:
        exe_path = path_;
        // TODO: working directory
        if (path_.filename() == path_) exe_path = fs::path(".") / exe_path;
    } else if (file_type == FileType::C || file_type == FileType::Cpp) {
        exe_path = path_;
        exe_path.replace_extension(OIGEN_EXE_EXTENSION);
        if (!compiled_ || config_.GetRecompiling()) {
            Compiler compiler(file_type, config_);
            result = compiler.Compile(path_, exe_path);
            if (result) return result; // TODO: Magic Number
            compiled_ = true;
        }
        // TODO: working directory
        if (path_.filename() == path_) exe_path = fs::path(".") / exe_path;
    } else if (file_type == FileType::Python) {
        exe_path = config_.GetInterpreterPython();
        arguments = fmt::format("\"{}\" {}",
            path_.string(),
            arguments
        );
    } else {
        result = -1; // TODO: Magic Number
    }

    // TODO: Working Directory
    int watcher_result = WEXITSTATUS(system(fmt::format(
        "./watcher_unix \"\\\"{}\\\" {}\" \"{}\" \"{}\" \"{}\" {} {}",
        exe_path.string(),
        arguments,
        input_path.string(),
        output_path.string(),
        error_path.string(),
        config_.GetTimeLimit().count(),
        config_.GetMemoryLimit()
    ).c_str()));
    // TODO: Get Result
    if (watcher_result == 1) {
        std::cout << "Unable to create Process" << std::endl;
        return -1; // TODO: Magic Number
    }
    if (watcher_result == 2) {
        // std::cout << fmt::format("exit code is {}\n", exit_code) << std::endl;
        std::cout << "Runtime Error" << std::endl;
        return -1; // TODO: Magic Number
    }
    if (watcher_result == 3) {
        std::cout << "Time Limit Exceeded" << std::endl;
        return -1; // TODO: Magic Number
    }
    if (watcher_result == 4) {
        std::cout << "Memory Limit Exceeded" << std::endl;
        return -1; // TODO: Magic Number
    }
#else
    // TODO: Unix, Mac
    // TODO: Time Limit
    std::string arguments = extra_arguments;
    // TODO: Config
    if (!input_path.empty()) {
        arguments += fmt::format(" < \"{}\"", input_path.string());
    }
    if (!output_path.empty()) {
        arguments += fmt::format(" > \"{}\"", output_path.string());
    }
    if (!error_path.empty()) {
        arguments += fmt::format(" 2> \"{}\"", error_path.string());
    }
    fs::path exe_path;
    if (file_type == FileType::Exe) { // TODO:
        exe_path = path_;
        // TODO: working directory
        if (path_.filename() == path_) exe_path = fs::path(".") / exe_path;
        result = system(fmt::format("\"{}\" {}", exe_path.string(), arguments).c_str());
    } else if (file_type == FileType::C || file_type == FileType::Cpp) {
        exe_path = path_;
        exe_path.replace_extension(OIGEN_EXE_EXTENSION);
        if (!compiled_ || config_.GetRecompiling()) {
            Compiler compiler(file_type, config_);
            result = compiler.Compile(path_, exe_path);
            if (result) return result; // TODO: Magic Number
            compiled_ = true;
        }
        // TODO: working directory
        if (path_.filename() == path_) exe_path = fs::path(".") / exe_path;
        result = system(fmt::format("\"{}\" {}", exe_path.string(), arguments).c_str());
    } else if (file_type == FileType::Python) {
        // TODO: set python interpreter path
        result = system(fmt::format("\"{}\" {} {}",
            config_.GetInterpreterPython(),
            path_.string(),
            arguments
        ).c_str());
    } else {
        result = -1; // TODO: Magic Number
    }
#endif
    return result;
}

FileType Runner::FindFileType(const fs::path &path) {
    auto type_iter = kExtFileType.find(path.extension().string());
    if (type_iter == kExtFileType.end()) return FileType::Unknown;
    return type_iter->second;
}

// TODO: config
const std::unordered_map<std::string, FileType> Runner::kExtFileType = {
    {".exe", FileType::Exe}, // Windows
    {".out", FileType::Exe}, // Linux
    {".c", FileType::C}, // TODO: all c/cpp extension
    {".cpp", FileType::Cpp},
    {".cc", FileType::Cpp},
    {".cxx", FileType::Cpp},
    {".py", FileType::Python},
};
