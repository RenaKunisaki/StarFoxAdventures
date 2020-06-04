#!/bin/sh
TOOL=/opt/devkitpro/devkitPPC/bin/powerpc-eabi

$TOOL-as -I include -mbroadway -mregnames -o $1.elf $1.s
$TOOL-objcopy -O binary $1.elf $1.bin
rm $1.elf
