Nullix: Null, not UNIX
Features:
- GRUB multiboot ELF boot
- IDT + PIC remap
- IRQ1 keyboard handler (buffered)
- VGA text printing (`vga_printf`, `vga_putc`)
- Buffered `getchar()` that blocks with `hlt`
- Basic memory management (kmalloc bump allocator)
- No userspace, no paging, no libc

Build & run (requires i686-elf toolchain, nasm, qemu in ./tools):

First, ensure tools are in ./tools/bin (i686-elf-gcc, nasm, qemu-system-i386).

Then set up the environment:
```sh
./buildenv.sh
```

Then build and run:
```sh
make
make run    # runs qemu with -kernel nullix.elf (multiboot direct boot)
```


If tools are missing, the script will error. For Windows, use Cygwin/MSYS to run the script. You can also download an tools.zip tarball for Windows.