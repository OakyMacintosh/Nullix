#include "../include/common.h"

// Syscall numbers
#define SYS_EXIT 1
#define SYS_WRITE 4

// Syscall dispatcher (called from assembly handler)
int syscall_dispatch(int num, ...) {
    switch (num) {
        case SYS_WRITE:
            // Implement write syscall
            break;
        case SYS_EXIT:
            // Implement exit
            break;
        default:
            return -1;
    }
    return 0;
}