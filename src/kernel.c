#include "../include/common.h"
#include "../include/kernel.h"

#define CODE_BUF_SIZE 256
static u8 code_buf[CODE_BUF_SIZE];
static u32 code_len = 0;

// Parse hex string to bytes
u32 parse_hex(const char* str, u8* out, u32 max_len) {
    u32 len = 0;
    while (*str && len < max_len) {
        if (*str == ' ') { str++; continue; }
        u8 hi = (*str >= '0' && *str <= '9') ? *str - '0' : (*str >= 'a' && *str <= 'f') ? *str - 'a' + 10 : (*str >= 'A' && *str <= 'F') ? *str - 'A' + 10 : 0;
        str++;
        u8 lo = (*str >= '0' && *str <= '9') ? *str - '0' : (*str >= 'a' && *str <= 'f') ? *str - 'a' + 10 : (*str >= 'A' && *str <= 'F') ? *str - 'A' + 10 : 0;
        str++;
        out[len++] = (hi << 4) | lo;
    }
    return len;
}

void kernel_main() {
    kinit();
    ramfs_mount(512 * 1024); // 512KB RAMFS
    kprint("Nullix is starting...\n");
    kprint("Welcome to kernelland.\n> ");
    kprint("Use 'hlp' for help.\n> ");
    while (1) {
        int c = kgetchar();
        if (c == '\r' || c == '\n') {
            kputc('\n');
            // Parse command
            if (code_len > 3 && code_buf[0] == 'r' && code_buf[1] == 'u' && code_buf[2] == 'n') {
                u8 bin[CODE_BUF_SIZE];
                u32 bin_len = parse_hex((char*)code_buf + 4, bin, CODE_BUF_SIZE);
                if (bin_len > 0) {
                    load_runtime_function(bin, bin_len);
                    kprint("Executed.\n");
                } else {
                    kprint("Invalid hex.\n");
                }
                code_len = 0;
            } else if (code_len >= 4 && code_buf[0] == 'd' && code_buf[1] == 'e' && code_buf[2] == 'm' && code_buf[3] == 'o') {
                demo_runtime();
                kprint("Demo executed.\n");
                code_len = 0;
            } else if (code_len >= 10 && code_buf[0] == 'm' && code_buf[1] == 'o' && code_buf[2] == 'u' && code_buf[3] == 'n' && code_buf[4] == 't' && code_buf[5] == 'r' && code_buf[6] == 'a' && code_buf[7] == 'm' && code_buf[8] == 'f' && code_buf[9] == 's') {
                // mountramfs <size>
                u32 size = 0;
                for (u32 i = 11; i < code_len && code_buf[i] >= '0' && code_buf[i] <= '9'; ++i) {
                    size = size * 10 + (code_buf[i] - '0');
                }
                if (size > 0) {
                    ramfs_mount(size * 1024);
                    kprint("RAMFS mounted with ");
                    // Simple print size
                    char buf[16];
                    u32 n = size;
                    u32 len = 0;
                    do {
                        buf[len++] = '0' + (n % 10);
                        n /= 10;
                    } while (n);
                    for (u32 i = len; i > 0; --i) kputc(buf[i-1]);
                    kprint("KB.\n");
                } else {
                    kprint("Invalid size.\n");
                }
                code_len = 0;
            } else if (code_len >= 6 && code_buf[0] == 'c' && code_buf[1] == 'r' && code_buf[2] == 'e' && code_buf[3] == 'a' && code_buf[4] == 't' && code_buf[5] == 'e') {
                // create <name> <data>
                // Simple: find space, then space
                u32 i = 7; // after "create "
                u32 name_start = i;
                while (i < code_len && code_buf[i] != ' ') ++i;
                code_buf[i] = 0; // null terminate name
                u32 data_start = i + 1;
                if (ramfs_create((char*)code_buf + name_start, code_buf + data_start, code_len - data_start) == 0) {
                    kprint("File created.\n");
                } else {
                    kprint("Failed to create file.\n");
                }
                code_len = 0;
            } else if (code_len >= 4 && code_buf[0] == 'r' && code_buf[1] == 'e' && code_buf[2] == 'a' && code_buf[3] == 'd') {
                // read <name>
                u32 size;
                u8* data = ramfs_read((char*)code_buf + 5, &size);
                if (data) {
                    kprint("File content: ");
                    for (u32 i = 0; i < size; ++i) kputc(data[i]);
                    kputc('\n');
                } else {
                    kprint("File not found.\n");
                }
                code_len = 0;
            } else if (code_len >= 5 && code_buf[0] == 'c' && code_buf[1] == 'l' && code_buf[2] == 'e' && code_buf[3] == 'a' && code_buf[4] == 'r') {
                code_len = 0;
                kprint("Cleared.\n");
            } else if (code_len >= 7 && code_buf[0] == 'l' && code_buf[1] == 'i' && code_buf[2] == 's' && code_buf[3] == 't' && code_buf[4] == 'f' && code_buf[5] == 's' && code_buf[6] == ' ') {
                // listfs <num>
                u32 fsnum = 0;
                for (u32 i = 7; i < code_len && code_buf[i] >= '0' && code_buf[i] <= '9'; ++i) {
                    fsnum = fsnum * 10 + (code_buf[i] - '0');
                }
                if (fsnum == 0) {
                    ramfs_list();
                } else {
                    kprint("Unknown filesystem.\n");
                }
                code_len = 0;
            } else if (code_len >= 10 && code_buf[0] == 'l' && code_buf[1] == 'i' && code_buf[2] == 's' && code_buf[3] == 't' && code_buf[4] == 'c' && code_buf[5] == 'o' && code_buf[6] == 'n' && code_buf[7] == 't' && code_buf[8] == 'e' && code_buf[9] == 'n' && code_buf[10] == 't') {
                // listcontent
                ramfs_list_content();
                code_len = 0;
            } else if (code_len >= 4 && code_buf[0] == 'e' && code_buf[1] == 'x' && code_buf[2] == 'e' && code_buf[3] == 'c') {
                // exec <name>
                u32 size;
                u8* data = ramfs_read((char*)code_buf + 5, &size);
                if (data) {
                    load_elf(data, size);
                    kprint("ELF execution finished.\n");
                } else {
                    kprint("File not found.\n");
                }
                code_len = 0;
            } else if (code_len >= 4 && code_buf[0] == 'e' && code_buf[1] == 'd' && code_buf[2] == 'i' && code_buf[3] == 't') {
                // edit <name> <data>
                u32 i = 5; // after "edit "
                while (i < code_len && code_buf[i] == ' ') ++i; // skip spaces
                if (i == code_len) {
                    kprint("Invalid edit command.\n");
                    code_len = 0;
                    continue;
                }
                u32 name_start = i;
                while (i < code_len && code_buf[i] != ' ') ++i;
                u32 name_end = i;
                code_buf[name_end] = 0;
                u32 data_start = name_end + 1;
                while (data_start < code_len && code_buf[data_start] == ' ') ++data_start;
                if (data_start > code_len) data_start = code_len;
                u32 size = code_len - data_start;
                u8* data_ptr = code_buf + data_start;
                u32 data_size = size;
                u8 bin_buf[CODE_BUF_SIZE];
                if (size >= 2 && code_buf[data_start] == '0' && code_buf[data_start + 1] == 'x') {
                    // Parse hex
                    data_size = parse_hex((char*)code_buf + data_start + 2, bin_buf, CODE_BUF_SIZE);
                    data_ptr = bin_buf;
                }
                // Show current
                u32 curr_size;
                u8* curr_data = ramfs_read((char*)code_buf + name_start, &curr_size);
                if (curr_data) {
                    kprint("Current: ");
                    for (u32 j = 0; j < curr_size; ++j) kputc(curr_data[j]);
                    kprint("\n");
                } else {
                    kprint("New file.\n");
                }
                if (ramfs_update((char*)code_buf + name_start, data_ptr, data_size) == 0) {
                    kprint("File updated.\n");
                } else {
                    kprint("Failed to update file.\n");
                }
                code_len = 0;
            } else if (code_len == 3 && code_buf[0] == 'h' && code_buf[1] == 'l' && code_buf[2] == 'p') {
                kprint("Commands:\n 'run <hex>' to execute code, 'demo' for example.\n 'mountramfs <size>' to mount RAMFS\n 'create <name> <data>' to create file\n 'read <name>' to read\n 'listfs <num>' to list files in fs\n 'listcontent' to list all file contents\n 'exec <name>' to execute ELF file\n 'edit <name> <data>' to edit file\n 'clear' to reset\n 'hlp' for help.\n");
                code_len = 0;
            } else {
                // Echo
                for (u32 i = 0; i < code_len; ++i) kputc(code_buf[i]);
                kputc('\n');
            }
            kprint("> ");
            code_len = 0;
        } else if (c == '\b') {
            if (code_len > 0) {
                code_len--;
                kputc('\b');
            }
        } else {
            if (code_len < CODE_BUF_SIZE - 1) {
                code_buf[code_len++] = (u8)c;
                kputc((char)c);
            }
        }
    }
}