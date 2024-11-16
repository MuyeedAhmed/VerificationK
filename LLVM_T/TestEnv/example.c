// clang -S -emit-llvm TestSet/Test.c -o TestSet/test.ll
#include <stdio.h>

int main() {
    int x = 42;
    printf("Value: %d\n", x);
    return 0;
}