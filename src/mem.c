#include "../include/common.h"

#define HEAP_START 0x00200000  // 2MB, after kernel
#define HEAP_SIZE 0x00100000   // 1MB heap

static u32 heap_ptr = HEAP_START;

void* kmalloc(u32 size) {
    if (heap_ptr + size > HEAP_START + HEAP_SIZE) return 0; // out of memory
    void* ptr = (void*)heap_ptr;
    heap_ptr += size;
    return ptr;
}

void kfree(void* ptr) {
    (void)ptr; // simple: no free
}