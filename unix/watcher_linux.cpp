#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <csignal>
#include <errno.h>
#include <cstring>
#include <cctype>
#include <thread>

#include "filesystem.h"
#include "fmt/core.h"

pid_t pid;
int pipefd[2];
void cleanUp(int) {
	kill(pid, SIGKILL);
	exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 7) {
        printf("Error: argc < 7\n");
        return 1;
    }
    int time_limit; // ms
    sscanf(argv[5], "%d", &time_limit);
    int memory_limit; // mb
    sscanf(argv[6], "%d", &memory_limit);
    // argv[1]: command
    // argv[2...4]: input/output/error file
    // argv[5...6]: time/memory limit
    pipe(pipefd);
    pid = fork();
    if (pid < 0) {
        return 1;
    } else if (pid > 0) {
		signal(SIGINT, cleanUp);
		signal(SIGABRT, cleanUp);
		signal(SIGTERM, cleanUp);

        // 1 cpu core
        if (!fs::exists("/sys/fs/cgroup/cpu/oigen_runner") &&
            !fs::create_directories("/sys/fs/cgroup/cpu/oigen_runner")) {
            return 1; // TODO: Magic Number
        }
        if (chmod("/sys/fs/cgroup/cpu/oigen_runner", 0755) == -1) {
            return 1; // TODO: Magic Number
        }
        system("echo 100000 > /sys/fs/cgroup/cpu/oigen_runner/cpu.cfs_quota_us");
        system("echo 100000 > /sys/fs/cgroup/cpu/oigen_runner/cpu.cfs_period_us");
        // memory limit
        if (memory_limit != -1) {
            if (!fs::exists("/sys/fs/cgroup/memory/oigen_runner") &&
                !fs::create_directories("/sys/fs/cgroup/memory/oigen_runner")) {
                return 1;// TODO: Magic Number
            }
            if (chmod("/sys/fs/cgroup/memory/oigen_runner", 0755) == -1) {
                return 1;// TODO: Magic Number
            }
            system(fmt::format(
                "echo {}M > /sys/fs/cgroup/memory/oigen_runner/memory.limit_in_bytes",
                memory_limit
            ).c_str());
            system(fmt::format(
                "echo {}M > /sys/fs/cgroup/memory/oigen_runner/memory.memsw.limit_in_bytes",
                memory_limit
            ).c_str());
        }
        // can't fork()/clone()
        if (!fs::exists("/sys/fs/cgroup/pids/oigen_runner")
            && !fs::create_directories("/sys/fs/cgroup/pids/oigen_runner")) {
            return 1;// TODO: Magic Number
        }
        if (chmod("/sys/fs/cgroup/pids/oigen_runner", 0755) == -1) {
            return 1; // TODO: Magic Number
        }
        system("echo 2 > /sys/fs/cgroup/pids/oigen_runner/pids.max");
        
        system(fmt::format("echo {} > /sys/fs/cgroup/cpu/oigen_runner/tasks", pid).c_str());
        if (memory_limit != -1) {
            system(fmt::format("echo {} > /sys/fs/cgroup/memory/oigen_runner/tasks", pid).c_str());
        }
        system(fmt::format("echo {} > /sys/fs/cgroup/pids/oigen_runner/tasks", pid).c_str());

        close(pipefd[1]);
        
        int container_status;

        auto start_time = std::chrono::steady_clock::now(), end_time = start_time;
        bool successful_finished = false;
        struct rusage usage {};
        while (end_time - start_time <= std::chrono::milliseconds(time_limit)) {
            pid_t ret_pid = wait4(pid, &container_status, WNOHANG, &usage);
            if (ret_pid == -1) {
                // printf("%d\n", errno);
                return 2; // TODO: Magic Number
            }
            if (ret_pid > 0) {
                successful_finished = true;
                break;
            }
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(10ms); // TODO: config
            end_time = std::chrono::steady_clock::now();
        }
        if (!successful_finished ||
            WTERMSIG(container_status) == SIGXCPU) {
            // TODO: Check kill
            printf("TLE!!!");
            kill(pid, SIGKILL);
            return 3; // TODO: Magic Number
        }
        // printf("%d\n", static_cast<int>(
        //     usage.ru_utime.tv_sec * 1000 +
        //     usage.ru_utime.tv_usec / 1000
        // )); // ms
        // printf("%d\n", static_cast<int>(usage.ru_maxrss)); // kb
        int exit_code = container_status >> 8;
        if ((memory_limit != -1 && usage.ru_maxrss > memory_limit * 1024) ||
            WTERMSIG(container_status) == SIGKILL) {
            printf("MLE!!!");
            return 4; // TODO: Magic Number
        }
        if (exit_code) {
            // printf("%d\n", exit_code);
            return 2; // TODO: Magic Number
        }
        return 0;
    } else {
        char ch;
        close(pipefd[1]);
        read(pipefd[0], &ch, 1);
        if (argv[2][0] != '\0') freopen(argv[2], "r", stdin);
        if (argv[3][0] != '\0') freopen(argv[3], "w", stdout);
        if (argv[4][0] != '\0') freopen(argv[4], "w", stderr);

        // TODO: How to kill this program?
        rlimit timlim{};
        rlim_t real_time_limit = (time_limit - 1) / 1000 + 2; // TODO: to second
        timlim = (rlimit){real_time_limit, real_time_limit + 1};
        setrlimit(RLIMIT_CPU, &timlim);
        execl("/bin/sh", "sh", "-c", argv[1], NULL);
        return 1;
    }
}
