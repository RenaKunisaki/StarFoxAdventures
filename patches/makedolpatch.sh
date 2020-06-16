#!/bin/bash
TOOL=/opt/devkitpro/devkitPPC/bin/powerpc-eabi

$TOOL-as -I include -mbroadway -mregnames --fatal-warnings -o $1.elf $1.s
if [ $? -ne 0 ]; then
   exit 1
fi

$TOOL-objcopy -O binary $1.elf $1.bin
rm $1.elf
