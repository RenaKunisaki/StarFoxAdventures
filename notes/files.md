MODELIND.bin seems to map model IDs to other IDs
u16 per model, model ID to use; 0=don't remap?
the code doesn't handle 0 specially so I guess 0=test cube

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
