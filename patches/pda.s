# PDA patch:
# Replaces the "PDA On/Off" item with a menu with lots of handy features.
# TODO: change the "PDA On/Off" text.
.text
.include "common.s"
.include "globals.s"

# define patches
patchList:
    PATCH_ID "PDAMenu" # must be 7 chars
    PATCH_BL 0x80133A54, itemHook
    PATCH_MAIN_LOOP mainLoop
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set MENU_WIDTH,300
    .set MENU_HEIGHT,220
    .set MENU_XPOS,320-(MENU_WIDTH/2)
    .set MENU_YPOS,240-(MENU_HEIGHT/2)
    .set LINE_HEIGHT,18
    .set MOVE_DELAY,15 # wait this many frames between autorepeat
    .set STACK_SIZE,0x180 # how much to reserve
    .set SP_LR_SAVE,0x184 # this is what the game does
    .set SP_STR_BUF,0x20 # temporary string buffer
    .set SP_FLOAT_TMP,0xA0 # temporary storage for float conversion (8 bytes)
    .set SP_GPR_SAVE,0xA8

entry: # called as soon as our patch is loaded.
    # nothing to do
    blr

# include various submodules.
# order is semi-important here.
# itemhook and main should come first. it might not hurt if they don't,
# but why take the chance?
# rearranging the submodules here will not change the order of
# the items displayed in-game; for that, edit `items.s`.
.include "pda/itemhook.s"
.include "pda/main.s"

.include "pda/hud.s"
.include "pda/animation.s"
.include "pda/player.s"
.include "pda/pdahud.s"
# unfortunately this isn't safe.
# playing invalid sounds can crash the game.
#.include "pda/soundtest.s"
.include "pda/debug.s"
.include "pda/bigmap.s"
.include "pda/mapalpha.s"
.include "pda/fov.s"
.include "pda/gamespeed.s"
.include "pda/volume.s"
.include "pda/util.s"
.include "pda/items.s"

# for proper memory alignment, this file must be included last.
.include "pda/vars.s"