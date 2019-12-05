# StarFoxAdventures
Reverse engineering notes for SFA

All work here is being done on USA v1.0. `GSAE01.map` is automatically exported from Ghidra and contains symbols found by Dolphin as well as by me.

## Eventual goals

* Play as Krystal throughout the game
    * Reimplement two-character mechanic
* Rip sound effects, textures, models
* Replace models?
* Find any interesting hidden goodies
    * Get the lighting debugger working, if it's able to work at all
    * Get `animtest` and any other unused maps to load

## Subprojects

### Debugger
This program connects to a USB Gecko interface and allows for reading and writing the game's memory. It provides some higher-level game-specific functions such as listing all loaded objects and the heap contents.
Right now it only works with a modified `codehandler.bin` in Dolphin. This handler doesn't wait for confirmation from the client before sending more data, which makes it much faster, but probably too unreliable to use on real hardware. Eventually I intend to adapt it to work with both versions.
This handler does still support Gecko codes just fine, though, and shouldn't interfere with normal operation of this or any other games.
It requires the Gecko be connected to memory card slot B. (See Dolphin's Gamecube settings.)

### Extractor
This program extracts resources from the game files. It's very much a work in progress and probably doesn't work well for many files yet. It might undergo massive changes including the interface.

### Modelviewer
This program attempts to render model data extracted from the game files. It's still very limited:
- Doesn't extract the model itself (use the Extractor program for that)
- Doesn't decode textures (loads them from a PNG file)
- Doesn't support animations
- Does display the model's geometry and skeleton
- Navigate by dragging the mouse buttons, arrow keys, A, and S
- Very likely to not work on any machine besides my own without a bit of hacking
- Also includes a nifty GLSL Python framework I made which can be useful for other projects (and in fact was ripped from one)
