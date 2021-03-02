#!/bin/bash
echo "Deleting:"

echo ".build"
rm -r .build

#echo "patched.rom"
#rm patched.rom

find . -name *.o -print -delete

echo "Done."
