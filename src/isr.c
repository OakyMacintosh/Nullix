#include "../include/common.h"

struct idt_entry {
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

#define IDT_ENTRIES 256
static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;

extern void syscall_handler();

static void set_idt_gate(int n, unsigned int handler) {
    idt[n].base_lo = handler & 0xFFFF;
    idt[n].sel = 0x08; // kernel code segment
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; // present, ring 0, 32-bit interrupt gate
    idt[n].base_hi = (handler >> 16) & 0xFFFF;
}

void init_idt() {
    idtp.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
    idtp.base = (unsigned int)&idt;
    for (int i = 0; i < IDT_ENTRIES; ++i) {
        set_idt_gate(i, 0);
    }
    set_idt_gate(0x21, (unsigned int)irq1_handler); // Keyboard
    set_idt_gate(0x80, (unsigned int)syscall_handler); // Syscall
    idt_load((unsigned int)&idtp);
}

void init_pic() {
    /* Remap PIC: master 0x20, slave 0x28 */
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    /* unmask keyboard IRQ (IRQ1) */
    outb(0x21, 0xFD); /* 11111101 - mask all except IRQ1 */
    outb(0xA1, 0xFF);
}

void enable_interrupts() {
    __asm__ volatile ("sti");
}