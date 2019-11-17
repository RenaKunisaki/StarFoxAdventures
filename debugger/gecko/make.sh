#!/bin/sh
DEVKITPPC_PATH=/opt/devkitpro/devkitPPC/bin
DOLPHIN_PATH=/usr/local/share/dolphin-emu/sys

$DEVKITPPC_PATH/powerpc-eabi-gcc -mpowerpc -mpaired -mbig codehandler.s -nostartfiles -nostdlib -nodefaultlibs -Wl,-Ttext,0x80001800 -o codehandler.elf
$DEVKITPPC_PATH/powerpc-eabi-strip --strip-debug --strip-all --discard-all -o codehandlers.elf -F elf32-powerpc codehandler.elf
$DEVKITPPC_PATH/powerpc-eabi-objcopy -I elf32-powerpc -O binary codehandlers.elf codehandler.bin
if [ ! -f $DOLPHIN_PATH/codehandler.bin.old ]; then
    sudo cp $DOLPHIN_PATH/codehandler.bin $DOLPHIN_PATH/codehandler.bin.old
fi
sudo cp codehandler.bin $DOLPHIN_PATH/codehandler.bin
