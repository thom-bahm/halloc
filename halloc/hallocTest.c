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

int main(int argc, char* argv[]) {
    printf("\nInit Halloc\n");
    initHalloc(BEST_FIT);
    dump();

    test();
}