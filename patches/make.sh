#!/bin/sh
TOOL=/opt/devkitpro/devkitPPC/bin/powerpc-eabi

$TOOL-as -mbroadway -o $1.elf $1.s
$TOOL-objcopy -O binary $1.elf $1.bin
./bin2gecko.py $1.bin $1.gecko
rm $1.elf $1.bin
