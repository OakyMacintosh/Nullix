BITS 32

global irq1_handler
global syscall_handler
extern keyboard_handler
extern syscall_dispatch

section .text
irq1_handler:
    pusha
    call keyboard_handler
    popa
    ; send EOI to PIC
    mov al, 0x20
    out 0x20, al
    iret

syscall_handler:
    pusha
    ; Syscall number in eax, args in ebx, ecx, etc.
    ; For now, just call dispatcher
    call syscall_dispatch
    popa
    iret