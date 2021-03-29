#!/bin/sh
TARGET=~/projects/sfa
ORIG=~/projects/sfa/original.iso
xdelta3 -d -f -s $ORIG patch.xdelta $TARGET/patched.iso
dolphin-emu -d $TARGET/patched.iso
