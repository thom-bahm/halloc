# halloc

halloc is a (currently VERY) simple memory allocator written in C. The current implementation is using a free list rather than mmap.

This project is being built mostly just to learn about dynamic memory allocation similar to existing functions in C such as malloc(), calloc(), realloc(), and free().

### Current task list:
- [x] halloc - simple malloc() replacement using free lists.
- [x] hfree - simple free() replacement.
- [x] first fit and best fit allocation strategies.
- [x] auto pick strategy based on bytes requested if AUTO strategy is set on initHalloc() call.
- [x] function to dump information about the heap.
- [ ] hrealloc() - simple realloc() replacement
- [ ] hcalloc() - simple calloc() replacement
