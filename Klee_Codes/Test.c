#include <klee/klee.h>
#include <assert.h>
#include <stdio.h>

int main() {
    float x, y;

    klee_make_symbolic(&x, sizeof(x), "x");
    klee_make_symbolic(&y, sizeof(y), "y");

    float result = x / y;

    klee_assert(y != 0.0f);

    printf("Result of division: %f\n", result);

    return 0;
}
