#!/bin/bash

echo "dsoxOS: Compiling boot loader..."
nasm -f bin boot/boot.asm -o boot/boot.bin

echo "dsoxOS: Compiling kernel..."
gcc -m32 -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -c kernel/kernel.c -o kernel/kernel.o
ld -m elf_i386 -T boot/linker.ld -nostdlib kernel/kernel.o -o kernel/kernel.bin

echo "dsoxOS: Creating OS image..."
cat boot/boot.bin kernel/kernel.bin > dsoxOS.bin

echo "dsoxOS: Build complete! Image size: $(stat -c%s dsoxOS.bin) bytes"
echo "dsoxOS: Run with: qemu-system-x86_64 -fda dsoxOS.bin"
