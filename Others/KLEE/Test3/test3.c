#include <stdio.h>
#include <stdbool.h>
#include <klee/klee.h>
#include<math.h>
#include <assert.h>

int main() {
    int x, y, r, x1, y1, d;
    int result_main;
    int result_student;

    klee_make_symbolic(&x, sizeof(x), "x");
    klee_make_symbolic(&y, sizeof(y), "y");
    klee_make_symbolic(&r, sizeof(r), "r");
    klee_make_symbolic(&x1, sizeof(x1), "x1");
    klee_make_symbolic(&y1, sizeof(y1), "y1");

    // Main
    d = (x1-x)*(x1-x) + (y1 - y)*(y1-y);
    // d = sqrtf(pow((x1-x), 2) + pow((y1 - y), 2));
    if(d < r*r)
        result_main = -1;
    else if(d == r*r)
        result_main = 0;
    else
        result_main = 1;

    // Student
    int s=(x-x1)*(x-x1)+(y-y1)*(y-y1);
    if(s>r)
        result_student = 1;
    else if(s==r)
        result_student = 0;
    else
        result_student = -1;
    
    klee_assert(result_main == result_student);

    return 0;
}