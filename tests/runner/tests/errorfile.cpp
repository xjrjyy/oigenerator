#include <cstdio>

int main() {
    int a, b;
    scanf("%d%d", &a, &b);
    printf("%d", a + b);
    fprintf(stderr, "%d", a - b);
    return 0;
}