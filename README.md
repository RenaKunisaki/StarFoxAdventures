# StarFoxAdventures
Reverse engineering notes for SFA.

Most of the information is in the wiki pages. This repository mainly contains code for patches, scripts, and extracted data.

## Directory Listing
### data
Contains a lot of information extracted from the game and/or written by hand, as well as a script that presents it in a web browser.

### debugger
A script that connects to a modified Gecko code handler (source included) to communicate with the game and read out state information, call functions, etc. Probably won't work on real hardware, but could be adapted to. (Modified code handler is because the original is painfully slow due to hardware limitations.)

### extractor
Old script for extracting various resources.

### isobuilder
Program for extracting, building, and patching GameCube ISOs.

### misc-scripts
Various small scripts I've used. Some are obsolete.

### modelviewer
Old, crude model viewer.
- Doesn't extract the model itself (use the Extractor program for that)
- Doesn't decode textures (loads them from a PNG file)
- Doesn't support animations
- Does display the model's geometry and skeleton
- Navigate by dragging the mouse buttons, arrow keys, A, and S
- Very likely to not work on any machine besides my own without a bit of hacking
- Also includes a nifty GLSL Python framework I made which can be useful for other projects (and in fact was ripped from one)

### notes
Unsorted notes, largely out of date.

### patches
Source for Amethyst Edition mod.

### GSAE01.map
Symbol map automatically exported from Ghidra. Symbol names made up by me for the most part.

### makeRelease.sh
Script to build Amethyst patch.
