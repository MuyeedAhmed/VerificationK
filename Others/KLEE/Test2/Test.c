#include <stdio.h>
#include <stdbool.h>
#include <klee/klee.h>

int main() {
    float a1, b1, a2, b2;
    float a, b;
    float d;
    bool inf_main, inf_b;

    klee_make_symbolic(&a1, sizeof(a1), "a1");
    klee_make_symbolic(&b1, sizeof(b1), "b1");
    klee_make_symbolic(&a2, sizeof(a2), "a2");
    klee_make_symbolic(&b2, sizeof(b2), "b2");

    d = a1*b2 - a2*b1;
	
	if (d==0) 
	    inf_main = true;
	else {
	    a = (b2-b1)/d;
	    b = (a1-a2)/d; 
        inf_main = false;
	}    

    float X=(b2-b1)/((b1*a2)-(a1*b2));/* x-coordinate of intersection point*/
    float Y=(a2-a1)/((b1*a2)-(a1*b2));/*y-coordinate of intersection point*/
    if (a1*b2==b1*a2)        /*condition for no intersection*/
    {
        inf_b = true;
    }
    else
    {
        inf_main = false;
    }

    klee_assume(inf_main != inf_b);

    if (inf_main != inf_b) {
        printf("Found values: a1=%.3f, b1=%.3f, a2=%.3f, b2=%.3f\n", a1, b1, a2, b2);
    }


    return 0;
}
