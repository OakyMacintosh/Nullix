# Toolchain
CC      := i686-elf-gcc
LD      := i686-elf-ld
AS      := nasm

# Flags
CFLAGS  := -ffreestanding -m32 -O2 -Wall -Wextra -nostdlib -fno-stack-protector -I include
ASFLAGS := -f elf32
LDFLAGS := -T linker.ld -m elf_i386

# Output
KERNEL  := nullix.elf

VERSION := 0.0.1
CURRENTDATE := $(shell date +%Y%m%d)

# Source files
C_SRC := \
    src/kernel.c \
    src/isr.c \
    src/keyboard.c \
    src/mem.c \
    src/vga.c \
    src/kernel_api.c \
    src/syscall.c \
    src/runtime.c \
    src/ramfs.c \
    src/fat.c \
    src/elf.c

ASM_SRC := \
    boot.s \
    src/irq.s \
    src/isr_stub.s

# Object files
OBJ := $(C_SRC:.c=.o) $(ASM_SRC:.s=.o)

# Default target
all: $(KERNEL)

# Link
$(KERNEL): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

# Compile C
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble ASM
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

# Clean
clean:
	rm -f $(OBJ) $(KERNEL)

run: $(KERNEL)
	qemu-system-i386 -kernel $(KERNEL) -m 512

mkgrub:
    if [ ! -d "mkgrub/boot/grub" ]; then mkdir -p mkgrub/boot/grub; fi
    cp $(KERNEL) mkgrub/boot/kernel.elf
    echo 'set timeout=0' > mkgrub/boot/grub/grub.cfg
    grub-mkrescue -o nullix-$(CURRENTDATE)-$(VERSION).iso mkgrub

.PHONY: all clean run