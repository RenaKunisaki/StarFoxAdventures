#!/bin/bash
# Script to compile a release of the mod.
TARGET=~/projects/sfa
ORIG=~/projects/sfa/original.iso
#TARGET=~/projects/games/hax/sfa/root
#ORIG=~/games/nintendo/gamecube/isos/Star\ Fox\ Adventures.iso
WORK=~/temp/sfawork

pushd ./patches
./makeall.sh
popd
echo '' # blank line

# gnu-as is butt and refuses to actually return an error code.
read -n 1 -p "Were there any errors? [y/n]" answer
if [ "$answer" != "n" ]; then exit 1; fi
echo '' # blank line
echo "Copying files..."

mkdir -p "$WORK/data/files"
mkdir -p "$WORK/data/sys"

# add any modified files here
mkdir -p "$WORK/data/files/gamefront"
#mkdir -p "$WORK/data/files/warlock"
#mkdir -p "$WORK/data/files/gametext/Warlock"
cp "$TARGET/files/gamefront/TEX0.tab" "$WORK/data/files/gamefront/TEX0.tab"
cp "$TARGET/files/gamefront/TEX0.bin" "$WORK/data/files/gamefront/TEX0.bin"
#cp "$TARGET/files/warlock/OBJSEQ.bin" "$WORK/data/files/warlock/OBJSEQ.bin"
#cp "$TARGET/files/warlock/ANIMCURV.bin" "$WORK/data/files/warlock/ANIMCURV.bin"
#cp "$TARGET/files/gametext/Warlock/English.bin" "$WORK/data/files/gametext/Warlock/English.bin"
cp "$TARGET/files/debug.bin" "$WORK/data/files/"
cp "$TARGET/files/WARPTAB.bin" "$WORK/data/files/"
cp -r "$TARGET/files/patches" "$WORK/data/files/"
cp "$TARGET/sys/main.dol" "$WORK/data/sys/"
find "$WORK" -name \*debug.log -delete

echo "Building ISO..."
cp "$ORIG" "$WORK/patched.iso"
./isobuilder/__main__.py overwrite patchIso "$ORIG" "$WORK/patched.iso" "$WORK/data"

echo "Creating patch..."
# using bigger compression window is necessary to get a patch that's
# ~700K instead of ~400M
xdelta3 -f -n -B 268435456 -s "$ORIG" "$WORK/patched.iso" patch.xdelta

echo "Tidying up..."
rm -rf "$WORK"
#zip -9v sfa-amethyst-1.0.zip patch.xdelta AmethystEditionReadme.md
