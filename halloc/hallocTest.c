#include "halloc.h"
#include <stdio.h>

void test() {
    printf("\nhalloc on a0: char* 128 bytes\n");
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
    printf("\nhalloc on a0: char* 128 bytes\n");
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

    printf("\nhrealloc: resize a0 (char* 128 bytes) to 256 bytes\n");
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

    printf("\nhrealloc on a1 (char* 256 bytes): resize to 64 bytes\n");
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
    dump(); 
    // at this point, should see a free block of size 128,256, and whatever other 
    // free block is in heap in the memory block output and in the Free list output
}


void test_calloc() {
    printf("\nhcalloc(5,4): Should be 20 bytes allocated to 0. 2nd element (index 1) should be 3.\n");
    int* arr = (int*) hcalloc(5, 4);
    arr[1] = 3;
    for (int i = 0; i < 4; i++) printf("%d\n", arr[i]);
    dump();

    printf("\nhcalloc(2,32): Should be 64 bytes allocated.\n");
    short* shortArr = (short*) hcalloc(2,32);
    for (short i = 0; i < 32; i++) {
        shortArr[i] = i;
    }
    for (int i = 0; i < 32; i++) printf("%d\n", shortArr[i]);
    dump();

    printf("\nhcalloc(0, 32): Should be 0 additional bytes allocated.\n");
    hcalloc(0, 32);
    dump();

    printf("\nhcalloc(2, 0): Should be 0 additional bytes allocated.\n");
    hcalloc(2, 0);
    dump();

}

int main(int argc, char* argv[]) {
    printf("\nInit Halloc\n");
    initHalloc(BEST_FIT);
    dump();
    test_calloc();
}