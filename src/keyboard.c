#include "../include/common.h"

#define KBD_BUF_SIZE 256
static char* kbd_buf;
static int kbd_head = 0, kbd_tail = 0;

/* simple US mapping for basic keys (partial) */
static const char scancode_map[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', /* backspace */
    '\t', /* tab */ 'q','w','e','r','t','y','u','i','o','p','[',']','\n', /* enter */
    0, /* ctrl */ 'a','s','d','f','g','h','j','k','l',';','\'', '`', 0, /* left shift */ '\\',
    'z','x','c','v','b','n','m',',','.','/', 0, /* right shift */ '*', 0, /* alt */ ' ', /* space */
};

void init_keyboard() {
    kbd_buf = kmalloc(KBD_BUF_SIZE);
}

void keyboard_handler() {
    unsigned char sc = inb(0x60);
    if (sc & 0x80) return; /* key release */
    char c = sc < 128 ? scancode_map[sc] : 0;
    if (c) {
        kbd_buf[kbd_head] = c;
        kbd_head = (kbd_head + 1) % KBD_BUF_SIZE;
    }
}

int getchar() {
    while (kbd_head == kbd_tail) {
        __asm__ volatile ("hlt");
    }
    char c = kbd_buf[kbd_tail];
    kbd_tail = (kbd_tail + 1) % KBD_BUF_SIZE;
    return (int)c;
}