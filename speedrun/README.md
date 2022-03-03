# SFA Speedrun Helper Mod v1.0

This is a mod made for speedrun testing, that copies a few helpful features from Amethyst while making sure not to change the game's heap layout. It does this by replacing a rather large file that the game loads but does nothing with (LACTIONS.bin) and having the game execute it.

## Display

Whenever Fox or Krystal are visible (excluding some cutscenes) some debug info is shown:

- L:XXXX XXXX: which two map IDs are currently loaded and which two are locked.
- Act:X Grp:XXXXXXXX: current map's act number and object group flags.
- M:XX XX: current free memory (slots, bytes) in percent.
- O:XXX: number of active objects.
- FPS:XX: current frame rate. should be 60 unless something is lagging.
- Vel: X.XXX X.XXX: Player's XZ and Y velocity.

Following is a table of position, rotation, and name of various objects:
- C: Camera
- P: Player
- R: Object the player is riding
- H: Object the player is holding
- T: Target object (has an A/C/heart icon above it)

After that some information is shown depending on the player's state, such as water currents and on-fire timer.

Some messages are also shown by the game itself; these will usually appear above the mod's display. The crash screen is also automatically enabled.


## Controls

Whenever the debug display is shown, you can press buttons on controller 3:

- Start: Toggle time freeze.
- Y: Advance one tick during time freeze.
- B: Abort current cutscene. Very likely to break things. Many will just immediately start again.
- X: Toggle most of the debug display so you can see the HUD.
- Z: Hold to fast-forward. (Will change physics behaviour.)
- L: Hold to slow down. (Will change physics behaviour.)


## Other Features

This mod also fixes a crash that happens when the game tries to report that an object's message queue is full.
