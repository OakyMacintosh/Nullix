#include "../include/common.h"

// Simple runtime function loader (flat binary, no ELF yet)
typedef void (*runtime_func_t)(void);

// Load and execute a flat binary function from memory
// For demo, assume code is in a buffer
void load_runtime_function(u8* code, u32 size) {
    // Allocate executable memory (in kernel, all is executable)
    void* func_addr = kmalloc(size);
    if (!func_addr) return;

    // Copy code
    for (u32 i = 0; i < size; ++i) {
        ((u8*)func_addr)[i] = code[i];
    }

    // Call the function
    runtime_func_t func = (runtime_func_t)func_addr;
    func();

    // For now, don't free (no kfree)
}

// Example: define a function at runtime (prints 'A' to VGA)
void demo_runtime() {
    // x86 code: mov byte [0xb8000], 'A'; ret
    u8 code[] = {
        0xC6, 0x05, 0x00, 0x80, 0x0B, 0x00, 0x41,  // mov byte [0xb8000], 0x41 ('A')
        0xC3  // ret
    };
    load_runtime_function(code, sizeof(code));
}