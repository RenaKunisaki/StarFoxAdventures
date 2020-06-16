#!/bin/bash
# make and install all patches
# you will probably want to change the destination
# and maybe the order
TARGET=~/projects/sfa/files

set -e
./makedolpatch.sh debugbin && mv -f debugbin.bin $TARGET/debug.bin
./makedolpatch.sh debugprint && mv -f debugprint.bin $TARGET/patches/0000
./makedolpatch.sh krystal && mv -f krystal.bin $TARGET/patches/0001
./makedolpatch.sh climb && mv -f climb.bin $TARGET/patches/0002
./makedolpatch.sh pausemenu && mv -f pausemenu.bin $TARGET/patches/0003
./makedolpatch.sh pda && mv -f pda.bin $TARGET/patches/0004
./makedolpatch.sh freemove && mv -f freemove.bin $TARGET/patches/0005
#./makedolpatch.sh alloc && mv -f alloc.bin $TARGET/patches/0006
#./makedolpatch.sh gamebit && mv -f gamebit.bin $TARGET/patches/0006
./makedolpatch.sh hitboxdebug && mv -f hitboxdebug.bin $TARGET/patches/0006

# extract Krystal model and texture to own files
dd if=$TARGET/animtest/MODELS.bin of=$TARGET/patches/km1 bs=1 skip=210720 count=76608
dd if=$TARGET/animtest/TEX1.bin of=$TARGET/patches/kt1 bs=1 skip=288864 count=104960
cp krystal/km2 krystal/kt2 $TARGET/patches/
