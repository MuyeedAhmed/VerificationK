#include <klee/klee.h>

void reverse_gt(char [], int, int);

int main() {
    char str[100], str_copy[100];
    const int size = 100;

    char out[1000];

    klee_make_symbolic(str, size, "str");
    str[size] = '\0';
    memcpy(str_copy, str, size + 1);

    reverse_gt(str, 0, size - 1);
    reverse_1

    return 0;
}

void reverse_gt(char str1[], int index, int size) {
    char temp;
    temp = str1[index];
    str1[index] = str1[size - index];
    str1[size - index] = temp;

    if (index >= size / 2) {
        return;
    }

    reverse_gt(str1, index + 1, size);
}

void str_rev_1(int n,char a[100])
{
    if(n==2) 
    printf("%c",a[0]);
    else 
    {
        printf("%c",a[n-2]);
        str_rev_1(n-1,a);
    }
}