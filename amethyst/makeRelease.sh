#!/bin/bash
# Script to compile a release of the mod.
TARGET=~/projects/sfa
ORIG=~/projects/sfa/original.iso
#TARGET=~/projects/games/hax/sfa/root
#ORIG=~/games/nintendo/gamecube/isos/Star\ Fox\ Adventures.iso
WORK=~/temp/sfawork

make clean
make install

# XXX should make the DOL patch and replace textures, etc, too

mkdir -p "$WORK/data/files"
mkdir -p "$WORK/data/sys"

# add any modified files here
mkdir -p "$WORK/data/files/gamefront"
cp "$TARGET/files/gamefront/TEX0.tab" "$WORK/data/files/gamefront/TEX0.tab"
cp "$TARGET/files/gamefront/TEX0.bin" "$WORK/data/files/gamefront/TEX0.bin"
cp "$TARGET/files/debug.bin" "$WORK/data/files/"
cp "$TARGET/files/bitnames.dat" "$WORK/data/files/"
cp "$TARGET/files/km0" "$WORK/data/files/"
cp "$TARGET/files/km1" "$WORK/data/files/"
cp "$TARGET/files/kt0" "$WORK/data/files/"
cp "$TARGET/files/kt1" "$WORK/data/files/"
cp "$TARGET/sys/main.dol" "$WORK/data/sys/"
find "$WORK" -name \*debug.log -delete

echo "Building ISO..."
cp "$ORIG" "$WORK/patched.iso"
../isobuilder/__main__.py overwrite patchIso "$ORIG" "$WORK/patched.iso" "$WORK/data"

echo "Creating patch..."
# using bigger compression window is necessary to get a patch that's
# ~700K instead of ~400M
xdelta3 -f -n -B 268435456 -s "$ORIG" "$WORK/patched.iso" patch.xdelta

echo "Tidying up..."
rm -rf "$WORK"
