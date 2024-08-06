#include "halloc.h"
#include <stdio.h>
#include <sys/mman.h>
#include <sys/resource.h>

#define HEAP_SIZE (128*8)

unsigned char heap[HEAP_SIZE];
int allocStrategy;

typedef struct blockHeader {
    int allocated; // 0 = free, 1 = allocated
    size_t size; // size of reserved block
    struct blockHeader* next;
} blockHeader;

// Head of the free list
blockHeader* head;

// initialize a heap
void initHalloc(int strategy) {
    allocStrategy = strategy;

    for (int i = 0; i < HEAP_SIZE; i++) {
        heap[i] = 0;
    }

    blockHeader* currblockHeader = (blockHeader*) heap;
    currblockHeader->allocated = 0; //initialize to free
    currblockHeader->size = HEAP_SIZE - sizeof(blockHeader);
    currblockHeader->next = NULL; // no next block yet
    head = currblockHeader;
}

void printBlockInfo() {
    printf("Memory Block\n");
    blockHeader* curr = (blockHeader*) heap;
    char memRep[HEAP_SIZE/8 +1]; //+1 for null terminator
    memRep[HEAP_SIZE/8] = '\0'; //set null terminator

    char freeChar = 'a';
    char usedChar = 'A';

    while ((unsigned char*) curr < heap + HEAP_SIZE) {
        if (!curr->allocated) {
            printf("free block at %p, size %zu\n", curr, curr->size);
            // set memory represenation to free character
            for (int i = 0; i  < (curr->size + sizeof(blockHeader)) / 8; i++) {
                memRep[((unsigned char*)curr-heap)/8 + i] = freeChar;
            }
            freeChar++;
        } else {
            printf("allocated block at %p, size %zu\n", curr, curr->size);
            for (int i =0; i < (curr->size + sizeof(blockHeader))/8; i++) {
                memRep[((unsigned char*) curr - heap) / 8 + i] = usedChar;
            }
            usedChar++;
        }
        curr = (blockHeader*)((unsigned char*) curr + curr->size + sizeof(blockHeader));
    }
    // print out memory representation;
    // lowercase characters indicate free chars, uppercase indicate allocated chars.
    printf("%s\n", memRep);
}

void printFreeList() {
    printf("Free list:\n");
    blockHeader* curr = head;
    while(curr != NULL) {
        printf("Block at %p, size %zu\n", curr, curr->size);
        curr = curr->next;
    }
}

// dump block info and free list
void dump() {
    printf("MEM DUMP\n");
    printBlockInfo();
    printFreeList();
}

void* halloc(size_t size) {
    // round up to the nearest multiple of 8
    int rem = size % 8;
    size_t alignedSize = size;
    if (rem != 0) alignedSize += (8-rem);

    int totalSize = alignedSize + sizeof(blockHeader);

    blockHeader* curr = head;
    blockHeader* prev = NULL;

    if (allocStrategy == FIRST_FIT) {
        while (curr != NULL) {
            // if we find a block that is not allocated and has enough space
            // we will use it
            if (!curr->allocated && curr->size >= totalSize) break;
        }
        prev = curr;
        curr = curr->next;
    }
    else if (allocStrategy == BEST_FIT) {
        blockHeader* bestFit = NULL;
        blockHeader* bestFitPrev = NULL;

        while (curr != NULL) {
            if (!curr->allocated && curr->size >= totalSize) {
                // if no bestFit has been found yet, set to current block. if not, 
                // set the bestFit block to the current if the current is smaller
                if (bestFit == NULL || curr->size < bestFit->size) {
                    bestFit = curr;
                    bestFitPrev = prev;
                }
            }
            prev = curr;
            curr = curr->next;
        }
        // set current block to the best we found
        curr = bestFit;
        prev = bestFitPrev;
    }

    // if no available blocks, return NULL
    if (curr == NULL) return NULL;
    
    blockHeader* newBlock = (blockHeader*) ((unsigned char*) curr + totalSize);
    
    newBlock->allocated = 0;
    newBlock->size = (curr->size) - totalSize;
    newBlock->next = curr->next;

    curr->size = alignedSize;
    curr->allocated = 1;

    if (curr == head) {
        head = newBlock;
        return (unsigned char*)curr + sizeof(blockHeader);
    }
    // remove current block from free list
    prev->next = newBlock;
    curr->next = NULL;
    return (unsigned char*)curr + sizeof(blockHeader);
}

void hfree(void *ptr) {
    // retrieve the block that is being requested to be freed.  
    blockHeader* block = (blockHeader*) ((unsigned char*) ptr - sizeof(blockHeader));
    block->allocated = 0; // free block

    blockHeader* curr = head;
    blockHeader* prev = NULL;
    // iterate from the head to the retrieved block
    while (curr != NULL && curr < block) {
        prev = curr;
        curr = curr->next;
    }
    // if previous block is null, the passed in block should be
    // the head of the free list
    if (prev == NULL) head = block;
    // otherwise, the block should be placed after the previous block
    else prev->next = block;

    // the freed block should now point to block which was past it
    block->next = curr;
} 