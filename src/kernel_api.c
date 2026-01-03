#include "../include/kernel.h"

// Forward declarations for internal functions
void vga_set_cursor_shape(int start, int end);
void init_keyboard();
void init_pic();
void init_idt();
void enable_interrupts();
int getchar();

// Kernel API implementations

void kprint(const char* s) {
    vga_write(s);
}

void kputc(char c) {
    vga_putc(c);
}

int kgetchar() {
    return getchar();
}

void kinit() {
    // Initialize kernel subsystems
    vga_clear();
    vga_set_cursor_shape(0, 15); // block cursor
    init_keyboard();
    init_pic();
    init_idt();
    enable_interrupts();
}