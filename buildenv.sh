#!/bin/bash

# Build environment setup for Nullix kernel
# Assumes tools are in ./tools (i686-elf-gcc, nasm, grub-mkrescue, qemu)
# Works with Cygwin/MSYS on Windows

set -e

TOOLS_DIR="$(pwd)/tools"
BIN_DIR="$TOOLS_DIR/bin"

if [ ! -d "$TOOLS_DIR" ]; then
    echo "Error: ./tools directory not found."
    echo "Please download and extract i686-elf toolchain, nasm, grub, qemu into ./tools"
    echo "Example: ./tools/bin/i686-elf-gcc, ./tools/bin/nasm, etc."
    exit 1
fi

echo "Setting up build environment from ./tools..."

# Add tools to PATH
export PATH="$BIN_DIR:$PATH"

# Check for required tools
REQUIRED_TOOLS="i686-elf-gcc nasm qemu-system-i386"
OPTIONAL_TOOLS="grub-mkrescue"
for tool in $REQUIRED_TOOLS; do
    if ! command -v "$tool" &> /dev/null; then
        echo "Error: $tool not found in PATH. Please ensure it's in ./tools/bin"
        exit 1
    fi
done
for tool in $OPTIONAL_TOOLS; do
    if ! command -v "$tool" &> /dev/null; then
        echo "Warning: $tool not found. ISO creation will fail; use 'make run' for direct boot."
    fi
done

echo "Build environment ready."
echo "Tools in PATH: $PATH"
echo "Run 'make' to build Nullix."