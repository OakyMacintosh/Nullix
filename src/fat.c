#include "../include/kernel.h"

// Basic FAT filesystem support (stub for now, assumes FAT12/16 image in buffer)

// FAT structures (simplified)
typedef struct {
    u8 jump[3];
    char oem[8];
    u16 bytes_per_sector;
    u8 sectors_per_cluster;
    u16 reserved_sectors;
    u8 num_fats;
    u16 root_entries;
    u16 total_sectors;
    u8 media;
    u16 sectors_per_fat;
    // etc.
} __attribute__((packed)) fat_bpb_t;

// Read file from FAT image in buffer
u8* fat_read_file(u8* fat_image, const char* name, u32* size) {
    // Stub: for now, return null
    // TODO: implement FAT parsing
    *size = 0;
    return 0;
}

// Mount FAT from buffer
void fat_mount(u8* buffer) {
    // Stub
    kprint("FAT mounted (stub).\n");
}