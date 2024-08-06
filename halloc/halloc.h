#include <stdlib.h>

#define AUTO -1
#define FIRST_FIT 0
#define BEST_FIT 1

void initHalloc(int strategy);
void *halloc(size_t size);
void *hcalloc(void *ptr, size_t size);
void *hrealloc(void *ptr, size_t size);

void hfree(void *ptr);

void dump();
