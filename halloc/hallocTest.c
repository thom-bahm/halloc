#include "halloc.h"
#include <stdio.h>

void test() {
    printf("\n halloc on a0: char* 128 bytes\n");
    char* a0 = (char*)halloc(128);
    if (a0 == NULL) {
        printf("halloc failed, returned NULL\n");
        exit(1);
    }
    dump();

    printf("\nhfree on a0: char* 128 bytes\n");
    hfree(a0);
    dump();
}

void test_realloc() {
    printf("\n halloc on a0: char* 128 bytes\n");
    char* a0 = (char*)halloc(128);
    if (a0 == NULL) {
        printf("halloc failed, returned NULL\n");
        exit(1);
    }
    dump();

    // Fill allocated memory with arbitrary data
    for (int i = 0; i < 128; i++) {
        a0[i] = 'A' + (i % 26);
    }

    printf("\n hrealloc: resize a0 (char* 128 bytes) to 256 bytes\n");
    char* a1 = (char*)hrealloc(a0, 256);
    if (a1 == NULL) {
        printf("hrealloc failed, returned NULL\n");
        exit(1);
    }
    dump();

    // Verify the data in the reallocated memory
    int errors = 0;
    for (int i = 0; i < 128; i++) {
        if (a1[i] != 'A' + (i % 26)) {
            errors++;
            printf("Data mismatch at index %d: expected %c, got %c\n", i, 'A' + (i % 26), a1[i]);
            exit(1);
        }
    }
    printf("\nNumber of errors after hrealloc on a0: %d\n", errors);

    printf("\n hrealloc on a1 (char* 256 bytes): resize to 64 bytes\n");
    char* a2 = (char*)hrealloc(a1, 64);
    if (a2 == NULL) {
        printf("hrealloc failed, returned NULL\n");
        exit(1);
    }
    dump();

    // Verify the data in the reallocated memory
    errors = 0;
    for (int i = 0; i < 64; i++) {
        if (a2[i] != 'A' + (i % 26)) {
            errors++;
            printf("Data mismatch at index %d: expected %c, got %c\n", i, 'A' + (i % 26), a2[i]);
            exit(1);
        }
    }
    printf("\nNumber of errors after hrealloc on a1: %d\n", errors);


    printf("\n hrealloc on a2: resize to 0 bytes\n");
    char* a3 = (char*)hrealloc(a2, 0);
    if (a3 != NULL) {
        printf("hrealloc failed, expected NULL\n");
        exit(1);
    }
    dump();
}


int main(int argc, char* argv[]) {
    printf("\nInit Halloc\n");
    initHalloc(BEST_FIT);
    dump();
    test_realloc();
}