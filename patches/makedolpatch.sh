#!/bin/bash
TOOL=/opt/devkitpro/devkitPPC/bin/powerpc-eabi

set -e
$TOOL-as -I include -mbroadway -mregnames --fatal-warnings -o $1.elf $1.s
$TOOL-objcopy -O binary $1.elf $1.bin
rm $1.elf
