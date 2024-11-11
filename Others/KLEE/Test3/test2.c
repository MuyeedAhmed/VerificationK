#include <stdio.h>
#include <stdbool.h>
#include <klee/klee.h>
#include <math.h>
#include <assert.h>

int main() {
    int x, d1, d2;

    klee_make_symbolic(&x, sizeof(x), "x");

    // Main
    d1 = pow(x, 2);
    d2 = pow(x, 3);

    klee_assert(d1 == d2);

    return 0;
}
