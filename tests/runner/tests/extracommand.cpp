#include <cstdio>
#include <cstdlib>

int main(int argc, char *argv[]) {
    int a = 0, b = 0;
    if (argc > 1) a = atoi(argv[1]);
    if (argc > 2) b = atoi(argv[2]);
    printf("%d %d", argc, a + b);
    return 0;
}