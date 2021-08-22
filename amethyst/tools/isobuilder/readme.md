# GameCube ISO Builder/Patcher

This program performs four functions:

1. Extract files from a GameCube ISO
2. Build a GameCube ISO from extracted files
3. Add and/or replace files in a GameCube ISO
4. Print information about a GameCube ISO

## Extracting files:
Usage: `./__main__.py extractIso game.iso outDir`

Will extract to two directories:
- `outDir/sys`: contains "system files" from the ISO
- `outDir/files`: contains the files from the ISO filesystem

"System files" are stored outside of the actual filesystem:
- apploader.img: the apploader program
- bi2.bin: boot and debug information
- boot.bin: disc header information
- fst.bin: the filesystem table
- main.dol: the main executable, loaded by the apploader

This is the same structure used by the Dolphin emulator.


## Building an ISO
Usage: `./__main__.py buildIso inDir game.iso`

`inDir` should contain the subdirectories `sys` and `files` as described in the previous section.


## Adding and replacing files
Usage: `./__main__.py patchIso original.iso new.iso patchDir`

`patchDir` should contain the subdirectories `sys` and `files` as described in the previous section. Files in these directories will be added to the ISO, replacing existing files at the same location.

At this time, there is no way to remove a file from the original ISO. Instead, you can replace it with a zero-byte file.

## Printing ISO information
Usage: `./__main__.py readIso game.iso`

Will print diagnostic information about the ISO contents, including a listing of its filesystem.
