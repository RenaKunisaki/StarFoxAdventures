This is the source code for the Star Fox Adventures: Amethyst Edition mod.

# Requirements
- python3
- devkitpro for GameCube
- the Star Fox Adventures v1.0 USA ISO
    - it should have the MD5 checksum `afb0306454b581e37a62170fdfd1de09`
    - place it in this directory and name it `original.iso`
- gnu make
- xdelta3 if you want to build a patch file

The build system assumes the existence of a `data` directory in the parent to this directory, containing information used for the debug menus.

# Building
Running `make` should handle verifying and extracting the ISO when necessary.

Running `make clean` will remove the build files, but not the extracted/patched ISO ir patch file.

Running `make install` will apply the patch to the extracted ISO, which can then be run in Dolphin (using `make run`).

Running `make iso` will build a patched ISO, which can be run in Dolphin or on a real console.

Running `make patch` will build the patched ISO as well as the xdelta file.

# Directory Structure

- bootstrap: source for `boot.bin` which the game will load at startup.
- build: temporary working directory.
- discroot: extracted ISO and modified files.
- dolpatch: source for DOL patch to load `boot.bin`.
- include: header files.
- patchfiles: modified binary files to replace those in the disc filesystem.
- src: main source code.
- tools: scripts used in building.
