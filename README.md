# OI Generator

OI Generator 是一款测试数据生成辅助工具，目前支持 Windows 和 Linux 平台。

你还在为生成数据而烦恼吗？只需提供数据生成器和标程，OI Generator 就可以帮助你快速生成大量测试数据。

## 入门

输入 `oigen(.exe) -h` 查看帮助。

使用方法：在同目录下放置 `gen.cpp` 和 `std.cpp`，开箱即用。

命令：`oigen(.exe) -n 20`（20 为数据组数）。

以下是 `gen.cpp` 和 `std.cpp` 的示例：

```cpp
// gen.cpp
#include <bits/stdc++.h>
using namespace std;

mt19937 rnd((unsigned int)chrono::system_clock::now().time_since_epoch().count());

int maxn = 1e9;
int a, b;

int main(int argc, char *argv[]) {
    if (argc > 1) {
        int new_maxn = atoi(argv[1]);
        if (new_maxn > 0) maxn = new_maxn;
    }
    a = rnd() % maxn, b = rnd() % maxn;
    printf("%d %d", a, b);
    return 0;
}
```

```cpp
// std.cpp
#include <cstdio>

int a, b;

int main() {
    scanf("%d%d", &a, &b);
    printf("%d", a + b);
    return 0;
}
```

测试数据默认生成在目录 `data` 下，默认对 C++ 文件使用 `{compiler} "{src}" -std=c++14 -O2 -o "{exe}"` 命令进行编译。

- 由于默认的 C++ 编译器路径是 `g++(.exe)`，如果你没有配置环境变量，需要使用 `--compiler-cpp {path_to_compiler}` 参数设置 C++ 编译器路径。
- 由于默认的 Python 解释器路径是 `python(.exe)`，如果你没有配置环境变量或需要使用 Python3，需要使用 `--interpreter-python {path_to_interpreter}` 参数设置 Python 解释器路径。
  - 使用 Python 支持时，不要忘了 `-g gen.py` 及 `-s std.py`。
  - 示例：`oigen(.exe) -g gen.py -s std.py --interpreter-python python(.exe)`。

## 进阶

当然，你也可以自行魔改 `start(.bat/.sh)`。

- 命令参数 `--show-id` 显示数据编号。
- 命令参数 `-f my{}data` 设置测试数据文件名，`{}` 内会填入数据编号。
- 命令参数 `--compiler-cpp g++` 设置 C++ 编译器目录。
- 命令参数 `-c {path_to_config}` 设置配置文件目录。
- 命令参数 `-t {time} -m {memory}` 设置运行时空限制。
  - 时间单位 ms，空间单位 mb。
  - 默认时间限制 2000ms，不设空间限制（为 -1）。
- 命令参数 `--print-used` 输出代码运行消耗时间和空间。
  - 目前还不支持区分 gen、std 和 usr。
- 命令参数 `-u {path_to_user_code}` 设置用户代码目录。
  - 设置了用户代码目录后，可以通过命令参数 `--compare` 开启对拍模式。
    - `--compare` 默认开启，但只有 `-u` 参数不为空时才有效。
  - 这是对拍命令示例：`oigen(.exe) -n 1000 -p chk -u usr.cpp -f data --show-id -t 1000 -m 256 --print-used`（你也可以在 `start2(.bat/.sh)` 里找到这条命令）。
  - 使用示例命令后，OI Generator 将会在 `chk` 目录下对拍 1000 次，并且展示数据编号。
  - 时间限制为 1000ms，空间限制为 256mb，输出代码运行消耗时间和空间。
  - 对拍默认使用全文比较，忽略文末空格和行末回车（目前暂不支持自定义比较器）。
  - 当**找到差异或程序运行超出限制**时，OI Generator 会提示是否继续，你可以选择继续或者退出对拍。

以下是配置文件示例：

```plain
10
100
1000
@3 这是一组部分分，一共 3 组测试数据
1000000
@4 这也是一组部分分，一共 4 组测试数据
100000000
@10 可以在 '@' 后面写点东西（这组部分分会这样运行：`gen.exe 1000000000`）
1000000000
```

以上配置最多生成 20 组测试数据，多余的配置会自动舍弃（不会调用数据生成器）。

如果你要生成的测试数据多于 20 组，会自动补齐（多于 20 组的部分命令行参数为空）。

你可以这样生成：`oigen(.exe) -n 20 -c conf.txt`（`conf.txt` 为配置文件名）。

## 使用开源项目

- [Catch2](https://github.com/catchorg/Catch2)

- [cxxopts](https://github.com/jarro2783/cxxopts)

- [filesystem](https://github.com/gulrak/filesystem)

- [fmt](https://github.com/fmtlib/fmt)
