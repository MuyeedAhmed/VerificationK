#include <klee/klee.h>
#include <stdio.h>
#include <string.h>

void reverse(char [], int, int);
void str_rev(int n, char a[100]);

char output_ground[200];
char output_student[200];
int index_ground = 0;
int index_student = 0;

void capture_ground(const char *format, char c) {
    output_ground[index_ground++] = c;
}

void capture_student(const char *format, char c) {
    output_student[index_student++] = c;
}

void reverse(char str1[], int index, int size) {
    char temp;
    temp = str1[index];
    str1[index] = str1[size - index];
    str1[size - index] = temp;

    if (index == size / 2) {
        return;
    }

    reverse(str1, index + 1, size);
}

void str_rev(int n, char a[100]) {
    if (n == 2) {
        capture_student("%c", a[0]);
    } else {
        capture_student("%c", a[n - 2]);
        str_rev(n - 1, a);
    }
}

int main() {
    char str[100], str_copy[100];
    int size;
    size = 100;
    // klee_make_symbolic(&size, sizeof(size), "size");
    // klee_assume(size > 0 && size < 100);

    klee_make_symbolic(str, size, "str");
    str[size] = '\0';

    memcpy(str_copy, str, size + 1);

    reverse(str, 0, size - 1);
    for (int i = 0; i < size; i++) {
        capture_ground("%c", str[i]);
    }

    str_rev(size + 1, str_copy);

    klee_assert(index_ground == index_student);
    for (int i = 0; i < index_ground; i++) {
        klee_assert(output_ground[i] == output_student[i]);
    }

    return 0;
}
