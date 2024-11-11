#include <stdio.h>
#include <stdbool.h>
#include <klee/klee.h>
#include <assert.h>

int main() {
    int a1, b1, a2, b2;
    int d, X, Y;
    bool inf_main, inf_b;

    klee_make_symbolic(&a1, sizeof(a1), "a1");
    klee_make_symbolic(&b1, sizeof(b1), "b1");
    klee_make_symbolic(&a2, sizeof(a2), "a2");
    klee_make_symbolic(&b2, sizeof(b2), "b2");
    d = a1 * b2 - a2 * b1;
    inf_main = (d == 0);

    inf_b = (a1 * b2 == a2 * b1);
    // klee_assert(b1 == 0);
    klee_assert(inf_main != inf_b);
    return 0;
}

