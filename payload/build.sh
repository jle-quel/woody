#!/bin/sh

rm -f stub
nasm -f elf64 $1.s && ld -m elf_x86_64 $1.o -o $1
rm -f *.o
