# OI Generator

OI Generator 是一款测试数据生成辅助工具。

你还在为生成数据而烦恼吗？只需提供数据生成器和标程，OI Generator 就可以帮助你快速生成大量测试数据。

## 入门

输入 `.\oigen.exe -h` 查看帮助。

使用方法：在同目录下放置 `gen.cpp` 和 `std.cpp`，开箱即用。

命令：`.\oigen.exe -n 20`（20 为数据组数）。

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

由于默认的 C++ 编译器路径是 `g++`，如果你没有配置环境变量，需要使用 `--compiler-cpp {path_to_compiler}` 参数设置 C++ 编译器路径。

## 进阶

当然，你也可以自行魔改 `start.bat`。

命令：`.\oigen.exe -f my{}data`  设置测试数据文件名，`{}` 内会填入数据编号。

命令：`.\oigen.exe --compiler-cpp g++` 设置 C++ 编译器目录。

命令：`.\oigen.exe -c {path_to_config}` 配置数据命令。

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

你可以这样生成：`.\oigen.exe -n 20 -c conf.txt`（`conf.txt` 为配置文件名）。

## 使用开源项目

- [Catch2](https://github.com/catchorg/Catch2)

- [cxxopts](https://github.com/jarro2783/cxxopts)

- [filesystem](https://github.com/gulrak/filesystem)

- [fmt](https://github.com/fmtlib/fmt)
