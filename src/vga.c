#include "../include/common.h"

volatile unsigned short* vga = (unsigned short*)0xB8000;
static u16 cursor_pos = 0;

void vga_putc(char c) {
    if (c == '\n') {
        cursor_pos += 80 - (cursor_pos % 80);
    } else {
        vga[cursor_pos++] = (u16) (c | (0x07 << 8));
    }
    vga_set_cursor_pos(cursor_pos);
}

void vga_write(const char* s) {
    for (const char* p = s; *p; ++p) vga_putc(*p);
}

void vga_clear() {
    for (u32 i = 0; i < 80*25; ++i) vga[i] = (u16)(' ' | (0x07 << 8));
    cursor_pos = 0;
    vga_set_cursor_pos(cursor_pos);
}

void vga_set_cursor_pos(u16 pos) {
    outb(0x3D4, 0x0F);
    outb(0x3D5, (u8)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (u8)((pos >> 8) & 0xFF));
}

void vga_set_cursor_shape(int start, int end) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (u8)start);
    outb(0x3D4, 0x0B);
    outb(0x3D5, (u8)end);
}