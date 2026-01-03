#include "../include/kernel.h"

int kstrcmp(const char* a, const char* b);

#define MAX_FILES 128
#define MAX_NAME_LEN 16
#define RAMFS_BLOCK_SIZE 512

typedef struct {
    char name[MAX_NAME_LEN];
    u32 size;
    u8* data;
} ramfs_file_t;

static ramfs_file_t files[MAX_FILES];
static u32 file_count = 0;
static u32 ramfs_size = 0;
static u8* ramfs_data = 0;

void ramfs_mount(u32 size) {
    if (ramfs_data) kfree(ramfs_data);
    ramfs_data = kmalloc(size);
    if (!ramfs_data) return;
    ramfs_size = size;
    file_count = 0;
    for (u32 i = 0; i < size; ++i) ramfs_data[i] = 0;
}

int ramfs_create(const char* name, const u8* data, u32 size) {
    if (file_count >= MAX_FILES) return -1;
    if (size > ramfs_size) return -1;
    u32 offset = 0;
    for (u32 i = 0; i < file_count; ++i) {
        offset += files[i].size;
    }
    if (offset + size > ramfs_size) return -1;
    for (u32 i = 0; i < size; ++i) {
        ramfs_data[offset + i] = data[i];
    }

    ramfs_file_t* f = &files[file_count++];
    for (u32 i = 0; name[i] && i < MAX_NAME_LEN - 1; ++i) f->name[i] = name[i];
    f->name[MAX_NAME_LEN - 1] = 0;
    f->size = size;
    f->data = ramfs_data + offset;
    return 0;
}

u8* ramfs_read(const char* name, u32* size) {
    for (u32 i = 0; i < file_count; ++i) {
        if (kstrcmp(files[i].name, name) == 0) {
            *size = files[i].size;
            return files[i].data;
        }
    }
    return 0;
}

int kstrcmp(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

void ramfs_list() {
    for (u32 i = 0; i < file_count; ++i) {
        kprint(files[i].name);
        kprint("\n");
    }
}

void ramfs_list_content() {
    for (u32 i = 0; i < file_count; ++i) {
        kprint("File: ");
        kprint(files[i].name);
        kprint("\nContent: ");
        for (u32 j = 0; j < files[i].size; ++j) {
            kputc(files[i].data[j]);
        }
        kprint("\n\n");
    }
}

int ramfs_update(const char* name, const u8* data, u32 size) {
    for (u32 i = 0; i < file_count; ++i) {
        if (kstrcmp(files[i].name, name) == 0) {
            if (size > files[i].size) {
                kprint("New size larger than current, truncating.\n");
                size = files[i].size;
            }
            for (u32 j = 0; j < size; ++j) {
                files[i].data[j] = data[j];
            }
            files[i].size = size;
            return 0;
        }
    }

    return ramfs_create(name, data, size);
}