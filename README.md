# OI Generator

OI Generator 是一款数据生成辅助工具。

## 入门

输入 `.\oigen.exe -h` 查看帮助。

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

使用方法：在同目录下放置 `gen.cpp` 和 `std.cpp`，开箱即用。

命令：`.\oigen.exe -n 20`（20 为数据组数）。

数据默认生成在目录 `data` 下，对 C++ 文件使用 `{compiler} "{src}" -std=c++14 -O2 -o "{exe}"` 命令进行编译。

## 进阶

当然，你也可以自行魔改 `start.bat`。

命令：`.\oigen.exe -f my{}data`  设置数据文件名，`{}` 内会填入数据编号。

命令：`.\oigen.exe --compiler-cpp g++` 设置 C++ 编译器目录。

命令：`.\oigen.exe -c conf.txt` 配置数据命令。以下是配置文件 `conf.txt`：

```plain
10
100
1000
@3 这是部分分，一共 3 个数据
1000000
@4 这也是部分分，一共 4 个数据
100000000
@10 可以在 '@' 后面写点东西（这组部分分会这样运行：`gen.exe 1000000000`）
1000000000
```

以上配置最多生成 20 组数据，多余的配置会自动舍弃。你可以这样生成：`.\oigen.exe -n 20 -c conf.txt`。

## 使用开源项目

- [Catch2](https://github.com/catchorg/Catch2)

- [cxxopts](https://github.com/jarro2783/cxxopts)

- [filesystem](https://github.com/gulrak/filesystem)

- [fmt](https://github.com/fmtlib/fmt)
