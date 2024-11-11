#include <klee/klee.h>

void reverse(char [], int, int);

int main() {
    char str[100];
    const int size = 100;

    klee_make_symbolic(str, size, "str");
    str[size - 1] = '\0';

    reverse(str, 0, size - 2);

    return 0;
}

void reverse(char str1[], int index, int size) {
    char temp;
    temp = str1[index];
    str1[index] = str1[size - index];
    str1[size - index] = temp;

    if (index >= size / 2) {
        return;
    }

    reverse(str1, index + 1, size);
}
