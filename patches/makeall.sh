#!/bin/bash
# make and install all patches
# you will probably want to change the destination
# and maybe the order
TARGET=~/projects/sfa
#TARGET=~/projects/games/hax/sfa/root/files

set -e
mkdir -p $TARGET/files/patches
rm -f $TARGET/files/patches/*
./makedolpatch.sh dolpatch
cp $TARGET/sys/main.dol.orig $TARGET/sys/main.dol
./patchFiles.py $TARGET

# extract Krystal model and texture to own files
dd if=$TARGET/files/animtest/MODELS.bin of=$TARGET/files/patches/km1 bs=1 skip=210720 count=76608
dd if=$TARGET/files/animtest/TEX1.bin of=$TARGET/files/patches/kt1 bs=1 skip=288864 count=104960
cp krystal/km2 krystal/kt2 $TARGET/files/patches/
