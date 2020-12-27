MODELIND.bin seems to map model IDs to other IDs
u16 per model, model ID to use; 0=don't remap?
the code doesn't handle 0 specially so I guess 0=test cube


Files we don't know about:
map/ANIMCURV.bin,tab
map/OBJSEQ2C.tab
map/OBJSEQ.bin,tab - probably scripting
map/VOXMAP.bin,tab
/BITTABLE.BIN
/CAMACTIONS.bin, CAMACTIO.bin
/CHAPBITS.bin - almost all zeros
/DLLS.tab - almost empty
/ENVFXACTIONS.bin, /ENVFXACT.bin
/FONTS.bin - used? format?
/GAMETEXT.bin,tab - used? format?
/HITS.bin,tab - related to map blocks?
/LACTIONS.bin
/MODANIM.BIN,TAB
/MODLINES.bin,tab
/OBJECTS.bin2 - used?
/OBJEVENT.bin
/OBJHITS.bin - object hitboxes?
/PREANIM.BIN,TAB
/SAVEGAME.bin,tab - used?
/SCREENS.bin,tab
/SPRITES.bin,tab
/SPRTABLE.bin
/TABLES.bin,tab - used?
/TEXPRE.bin,tab - related to PREANIM?
/TEXTABLE.bin
/TRKBLK.tab - related to map blocks
/WEAPONDA.bin

empty files:
/CACHEFONTSTAB.bin, /CACHEFONTSTEX.bin
/VOXOBJ.bin,tab


Files that are specific to each map:
.bin          |.tab        |Description
--------------|------------|-----------
ANIM.BIN      |ANIM.TAB    |character animations
ANIMCURV.bin  |ANIMCURV.tab|contains many "SEQA" headers
modXX.zlb.bin |modXX.tab   |? XX is different numbers for each map
MODELIND.bin  |(none)      |maps model IDs to different IDs (make them negative to prevent this)
MODELS.bin    |MODELS.tab  |Character models
(none)        |OBJSEQ2C.tab|?
OBJSEQ.bin    |OBJSEQ.tab  |lots of flags?
TEX0.bin      |TEX0.tab    |Textures, eg environment
TEX1.bin      |TEX1.tab    |Textures, eg characters
VOXMAP.bin    |VOXMAP.tab  |ZLB archives; related to map? (voxel?)

each of these names is also present in the disc root.
not sure if those are unused, or fallback, or some maps use them?
or used for some "other" maps that don't have their own files?
however, not every modXX.zlb.bin has a corresponding modXX.tab in the root.

each map has a directory containing those files, and also a `(mapname).romlist.zlb` in the disc root.

.bin/.tab files in disc root only:
.bin             |.tab        |Description
-----------------|------------|-----------
AMAP.BIN         |AMAP.TAB    |animation map?
BITTABLE.BIN     |(none)      |game bits?
CACHEFONTSTAB.bin|(none)      |empty file
CACHEFONTSTEX.bin|(none)      |empty file
CAMACTIO.bin     |(none)      |maybe old version of CAMACTIONS.bin? not quite identical
CAMACTIONS.bin   |(none)      |?
CHAPBITS.bin     |(none)      |almost all zeros
(none)           |DLLS.tab    |? almost empty
ENVFXACT.bin     |(none)      |old ENVFXACTIONS.bin? many differences
ENVFXACTIONS.bin |(none)      |?
FONTS.bin        |(none)      |presumably font graphics; they're named inside
GAMETEXT.bin     |GAMETEXT.tab|Old version of gametext, not used(?)
globalma.bin     |(none)      |old globalmap.bin? longer
globalmap.bin    |(none)      |?
HITS.bin         |HITS.tab    |object hitboxes?
LACTIONS.bin     |(none)      |?
MAPINFO.bin      |(none)      |name, type, ??? for each map
MAPS.bin         |MAPS.tab    |? possibly minimap graphics?
MODANIM.BIN      |MODANIM.TAB |?
MODLINES.bin     |MODLINES.tab|?
OBJECTS.bin      |OBJECTS.tab |object definitions incl names
OBJECTS.bin2     |(none)      |old unused version? many differences
OBJEVENT.bin     |(none)      |?
OBJHITS.bin      |(none)      |hitboxes? looks like a sparse table
OBJINDEX.bin     |(none)      |maps object IDs to different IDs
PREANIM.BIN      |PREANIM.TAB |?
SAVEGAME.bin     |SAVEGAME.tab|? mostly empty; .tab contains one entry
SCREENS.bin      |SCREENS.tab |looks like u16 width, height, pixels[] (but isn't?)
SPRITES.bin      |SPRITES.tab |? tiny file
SPRTABLE.bin     |(none)      |?
TABLES.bin       |TABLES.tab  |?
TEXPRE.bin       |TEXPRE.tab  |textures for something (sort of "TEX2")
TEXTABLE.bin     |(none)      |?
(none)           |TRKBLK.tab  |?
VOXOBJ.bin       |VOXOBJ.tab  |empty file and table
WARPTAB.bin      |(none)      |coords and ??? for each warp index to send to
WEAPONDA.bin     |(none)      |weapon data?

misc files in disc root:
File Name       |Description
----------------|-----------
apploader.img   |the bootloader (I might have put this here?)
boot.dol        |the main executable
openingXX.bnr   |(XX=EU,JP,US,(blank)) banner image for somewhere
splashScreen.bin|a 640x480 logo "StarFox Adventures - Dinosaur Planet" (is this used?)
starfox.thp     |title screen movie

misc dirs in disc root: (excluding maps)
Dir Name| Description
--------|------------
audio   |all sound effect archives, midi
card    |save file icons for display in console's memory card menu
gametext|the used gametext files for each map
modules |two files that appear to be unused
musyxbin|two empty dirs
savegame|save game files used for debug chapter select
streams |audio streams (long voice clips, music?)

tab files that differ between warlock/swaphol:
ANIM.TAB          0 differences
ANIMCURV.tab      0
MODELS.tab      137
OBJSEQ.tab        0
OBJSEQ2C.tab      0
TEX0.tab         27
TEX1.tab        199
VOXMAP.tab       75

CHAPBITS.bin (chapter bits?) is all zeros except:
00001ED0  00 00 00 00  00 00 00 00  00 00 00 00  01 00 00 00  
00001EE0  00 00 00 01  00 00 00 01  00 00 00 01  00 00 00 01  

00005660  00 00 00 00  00 00 00 00  01 00 00 00  08 00 04 2D  
00005670  08 00 04 2D  08 00 04 2D  08 00 04 2D  00 00 00 00  
0x42D = 1069


now that I got Dolphin to load extracted files let's try renaming some and seeing what happens.

(listed in order of renaming, I didn't rename back if no change)
CHAPBITS.bin: no change
GAMETEXT.bin: no change, but game does try to read it
GAMETEXT.tab: no change, but game does try to read it
OBJECTS.bin2: no change
BITTABLE.bin: Krystal is in a glitched pose when starting the game; some HUD elements are missing; my max scarabs is 10 instead of 100 but I still have 100
    this is actually just the table that gives the table index and offset of each GameBit.
CACHEFONTSTAB.bin: no change
CACHEFONTSTEX.bin: no change
CAMACTIO.bin: the camera starts higher up and moves down
CAMACTIONS.bin: no change
ENVFXACT.bin: LightFoot Village is not foggy
ENVFXACTIONS.bin: no change
FONTS.bin: no change, but game does try to read it
globlama.bin: crash at title screen at 80135060
globalmap.bin: no change
HITS.bin: can't climb some ledges? eg the small rectangular hole at LightFoot Village
HITS.tab: crash on loading save, at 800034ac (memcpy)
LACTIONS.bin: <camcontrol.c>  failed to load triggered camaction actionno 0
    actually that happens anyway when running in/out of the huts, might be related to previous changes or just normal
MAPINFO.bin: the PDA HUD appears on the title screen
MODANIM.BIN: crash at title screen at 800471ac (loadAndDecompressDataFile)
MODLINES.bin: no change, but game does try to read it
OBJEVENT.bin: no change, but game does try to read it
PREANIM.BIN: Krystal starts in a glitched pose; crash at 8028518c when trying to move
SAVEGAME.bin: no change, but game does try to read it
SAVEGAME.tab: no change, but game does try to read it
SCREENS.bin: no change
SCREENS.tab: no change, but game does try to read it
    makes me suspect SCREENS.bin is used somewhere...
SPRITES.bin: no change
SPRITES.tab: no change
SPRTABLE.bin: no change
TABLES.bin: no change, but game does try to read it
TABLES.tab: no change, but game does try to read it
    I wonder if it relates to high score tables?
TEXPRE.bin: crash at title screen "memory usage value corrupted"
TEXPRE.tab: crash at title screen in memcpy
TEXTABLE.bin: crash at title screen at 80054ac0
TRKBLK.tab: many map elements are missing or don't appear until you get close
    - the title screen is missing all its objects
    - on loading the save file at LightFoot Village, I spawn in the air and fall into a very incomplete map where it's possible to fall off the edge of the world
    - interestingly you actually fall instead of softlocking like you normally do when walking out of bounds
VOXOBJ.bin: no change
VOXOBJ.tab: no change
WEAPONDA.bin: the staff's swipe effect is misplaced, like it is when you hack Krystal to have it at the start of the game
splashScreen.bin: no change
starfox.thp: hang at title screen showing blank monitor
    Dolphin spams warning about not finding the file, so it's possible the game isn't really hung but is just lagging too badly to respond
    replacing it with a 0-byte file causes spam about reading out of the file, but the video still plays!? maybe it's reading into the next file which happens to be the renamed original video
modules: no change
musyxbin: no change
savegame: in the debug chapter select, all chapters lead to a new game, else, no change (the game never looks at these files otherwise)
streams: no voices play
audio: no sound at all, but game works
gametext: all text is blank, game can't be paused, talking to NPCs softlocks
MODELS.bin: no change
OBJHITS.bin: no change, but game does try to read it
DLLS.tab: no change

for some files eg GAMETEXT.bin, the game tries to read them and produces a warning that the file is missing, but it continues on fine. for others, there is no message, ie the game doesn't even read them.

anyway even with those missing, I can go from the very beginning to Krazoa Palace just fine.


let's try swapping some files
OBJECTS.bin2 -> OBJECTS.bin: crash at title in memcpy
    could we reconstruct the table by scanning for names?
    or use one from the demo version?
CAMACTIONS.bin -> CAMACTIO.bin: no change
ENVFXACTIONS.bin -> ENVFXACT.bin: LightFoot Village is less foggy; ThornTail Hollow's sky is just a dark blue gradient even in the day
globalmap.bin -> globalma.bin: crash at title at 80135060, after loading some files from worldmap and instantiating Krystal and CommandMenu
GAMETEXT.bin -> gametext/FrontEnd/English.bin: no text on title screen
warlock -> swaphol: swaphol's objects load but are all cubes; the camera does a weird spin at the beginning; the arwing is facing into the ground; you can't move
    let's also do warlock.romlist.zlb -> hollow2.romlist.zlb: no change
    let's do with hollow.romlist.zlb instead: GPU hang
    let's also rename mod16 -> mod13: still hang
    let's keep the dir as-is but rename the romlist: still hang
SAVEGAME.bin -> savegame/save1.bin: no change
SAVEGAME.bin -> savegame/save2.bin: you start in an empty void with 1/4 health
savegame/save1.bin -> savegame/save2.bin: you start in the Walled City
    apparently save1.bin doesn't actually get loaded.

let's try editing some files
MAPINFO.bin: ThornTail Hollow (param 00 06 00 00)
    P1 -> 03: no objects load. the camera is looking from the player spawn point, inside the ground, and can't move. using free camera to move around, most of the map isn't loaded. the sky is empty.
    P1 -> 01: no change
    P1 -> 02: same as 03
    P1 -> 04: crash at 802972b4 with a null ObjInstance
    P1 -> 05: no change
    P1 -> 06: no change
    P1 -> 07: no change, so this isn't flags
    P1 -> 7F: no change
    P2 -> 00: no change
    P2 -> 7F: no change
    P3 -> 7F: no change
    P -> 01 06 7F 7F: no change
    first letter of name -> Z: no change
    first letter lowercase: no change
    first letter -> null: no change
    this is strange since removing this file causes the PDA HUD to appear on the title screen

let's try changing the title screen params instead from 04 06 00 00
    P1 -> 00: the PDA HUD appears. so the crash might be related?
    P1 -> 01: same as 00
    P1 -> 02: no objects load
    P1 -> 03: same as 02
    P1 -> 05: same as 00
    P1 -> 7F: same as 00
    P1 -> FF: same as 00
    P2 -> 00: no change
    P3,4 -> AAAA: no change

my guess as to P1:
00: normal map
01: normal submap
02: special map, no objects (unused type)
03: special submap (only used by an unused map)
04: special map (title, Arwing)
of note, shipbattle doesn't hide the PDA HUD, because you normally play as Krystal there so it won't appear anyway. with the mod, which overrides character checks, it does appear.

also, should try editing some of the warp entries that lead to animtest, and changing the layer parameter or even coords
