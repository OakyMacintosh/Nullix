#ifndef COMMON_H
#define COMMON_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

static inline void outb(u16 port, u8 val) {
    __asm__ volatile ("outb %0, %1" : : "a" (val), "Nd" (port));
}

static inline u8 inb(u16 port) {
    u8 ret;
    __asm__ volatile ("inb %1, %0" : "=a" (ret) : "Nd" (port));
    return ret;
}

extern void irq1_handler();
extern void syscall_handler();
extern void idt_load(unsigned int);
void* kmalloc(u32 size);
void kfree(void* ptr);
void vga_clear();
void vga_write(const char* s);
void vga_putc(char c);
// void vga_set_cursor_shape(int start, int end);
void vga_set_cursor_pos(u16 pos);

#endif