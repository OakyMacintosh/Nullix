#include "../include/kernel.h"

typedef struct {
    u8 e_ident[16];
    u16 e_type;
    u16 e_machine;
    u32 e_version;
    u32 e_entry;
    u32 e_phoff;
    u32 e_shoff;
    u32 e_flags;
    u16 e_ehsize;
    u16 e_phentsize;
    u16 e_phnum;
    u16 e_shentsize;
    u16 e_shnum;
    u16 e_shstrndx;
} Elf32_Ehdr;

typedef struct {
    u32 p_type;
    u32 p_offset;
    u32 p_vaddr;
    u32 p_paddr;
    u32 p_filesz;
    u32 p_memsz;
    u32 p_flags;
    u32 p_align;
} Elf32_Phdr;

#define PT_LOAD 1
#define ET_EXEC 2
#define EM_386 3
#define EI_MAG0 0x7f
#define EI_MAG1 'E'
#define EI_MAG2 'L'
#define EI_MAG3 'F'

void load_elf(u8* elf_data, u32 size) {
    if (size < sizeof(Elf32_Ehdr)) {
        kprint("Invalid ELF size.\n");
        return;
    }
    Elf32_Ehdr* hdr = (Elf32_Ehdr*)elf_data;
    if (hdr->e_ident[0] != EI_MAG0 || hdr->e_ident[1] != EI_MAG1 ||
        hdr->e_ident[2] != EI_MAG2 || hdr->e_ident[3] != EI_MAG3) {
        kprint("Not an ELF file.\n");
        return;
    }
    if (hdr->e_machine != EM_386) {
        kprint("Not i386 ELF.\n");
        return;
    }
    if (hdr->e_type != ET_EXEC) {
        kprint("Not an executable ELF.\n");
        return;
    }
    if (hdr->e_phoff + hdr->e_phnum * sizeof(Elf32_Phdr) > size) {
        kprint("Invalid program headers.\n");
        return;
    }
    Elf32_Phdr* phdr = (Elf32_Phdr*)(elf_data + hdr->e_phoff);
    for (u16 i = 0; i < hdr->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            u32 vaddr = phdr[i].p_vaddr;
            u32 filesz = phdr[i].p_filesz;
            u32 memsz = phdr[i].p_memsz;
            u32 offset = phdr[i].p_offset;
            if (offset + filesz > size) {
                kprint("Invalid segment offset.\n");
                return;
            }

            for (u32 j = 0; j < filesz; j++) {
                ((u8*)vaddr)[j] = elf_data[offset + j];
            }

            for (u32 j = filesz; j < memsz; j++) {
                ((u8*)vaddr)[j] = 0;
            }
        }
    }

    ((void(*)())hdr->e_entry)();
}