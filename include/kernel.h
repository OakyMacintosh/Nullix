#ifndef KERNEL_H
#define KERNEL_H

#include "common.h"

// Kernel API functions

// VGA output
void kprint(const char* s);
void kputc(char c);

// Memory management
void* kmalloc(u32 size);
void kfree(void* ptr);

// Keyboard input
int kgetchar();

// Initialization
void kinit();

// Syscalls
int syscall_dispatch(int num, ...);

// Runtime functions
void load_runtime_function(u8* code, u32 size);
void demo_runtime();

// RAMFS
void ramfs_mount(u32 size);
int ramfs_create(const char* name, const u8* data, u32 size);
u8* ramfs_read(const char* name, u32* size);
void ramfs_list();
void ramfs_list_content();
int ramfs_update(const char* name, const u8* data, u32 size);

// FAT
void fat_mount(u8* buffer);
u8* fat_read_file(u8* fat_image, const char* name, u32* size);

// ELF
void load_elf(u8* data, u32 size);

#endif