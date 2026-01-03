BITS 32
ALIGN 4

; Multiboot header
section .multiboot
    dd 0x1BADB002          ; magic
    dd 0x00000003          ; flags: align modules, memory info
    dd -(0x1BADB002 + 0x00000003)

section .text
global _start
extern kernel_main

_start:
    jmp kernel_main