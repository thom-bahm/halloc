#include <stdlib.h>

#define AUTO -1
#define FIRST_FIT 0
#define BEST_FIT 1

void initHalloc(int strategy);
void *halloc(size_t size);
void *hcalloc(size_t numElem, size_t elemSize);
void *hrealloc(void *ptr, size_t new_size);

void hfree(void *ptr);

void dump();
