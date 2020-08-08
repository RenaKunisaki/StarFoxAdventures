# PDA patch:
# Replaces the "PDA On/Off" item with a menu with lots of handy features.
# TODO: change the "PDA On/Off" text.
.text
.include "common.s"
.include "globals.s"

# define patches
patchList:
    PATCH_ID   "PDAMenu" # must be 7 chars
    PATCH_BL   0x80133A54, itemHook
    PATCH_MAIN_LOOP        mainLoop
    PATCH_B    0x80018414, textHook
    PATCH_BL   0x800183dc, textDrawHook
    # disable special case for spaces in text.
    # it's not necessary anyway and interferes
    # with our fixed width hack.
    PATCH_WORD 0x80017C70, 0x2816ACAB
    PATCH_WORD 0x802B6414, 0x60000000 # disable normal backpack handling
    # hook into loadSaveSettings to repurpose some unused settings.
    PATCH_B    0x800e7f9c, saveLoadHook
    PATCH_END  PATCH_KEEP_AFTER_RUN

constants:
    .set MENU_WIDTH,     320
    .set MENU_HEIGHT,    230
    .set MENU_XPOS,      320-(MENU_WIDTH/2)
    .set MENU_YPOS,      240-(MENU_HEIGHT/2)
    .set LINE_HEIGHT,    18
    .set MOVE_DELAY,     10 # wait this many frames between autorepeat
    .set MENU_TEXTBOX_ID,0x93 # same one the game uses for HUD

    .set MENU_ID_MAIN,     0 # constants for whichMenu
    .set MENU_ID_OBJLIST,  1
    .set MENU_ID_GAMEBIT,  2
    .set MENU_ID_WARP,     3
    .set MENU_ID_HEAP_LIST,4
    .set MENU_ID_HEAP,     5
    .set MENU_ID_SAVEGAME, 6
    .set MENU_ID_TEXTURES, 7

    .set STACK_SIZE, 0x180 # how much to reserve
    .set SP_LR_SAVE, 0x184 # this is what the game does
    .set SP_ARG9,     0x08 # for eg sprintf
    .set SP_ARG10,    0x0C
    .set SP_ARG11,    0x10
    .set SP_STR_BUF,  0x40 # temporary string buffer
    .set SP_FLOAT_TMP,0xA0 # temporary storage for float conversion (8 bytes)
    .set SP_GPR_SAVE, 0xA8

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
.include "pda/hudelems.s"
.include "pda/drawbox.s"
.include "pda/animation.s"
.include "pda/gamesettings.s"
.include "pda/pdahud.s"
.include "pda/soundtest.s"
.include "pda/debug.s"
.include "pda/debugdata.s"
.include "pda/debugmap.s"
.include "pda/debugtext.s"
.include "pda/bigmap.s"
.include "pda/mapalpha.s"
.include "pda/volume.s"
.include "pda/util.s"
.include "pda/items.s"
.include "pda/objmenu.s"
.include "pda/gamebitmenu.s"
.include "pda/warpmenu.s"
.include "pda/heaplist.s"
.include "pda/heapmenu.s"
.include "pda/savegamemenu.s"
.include "pda/savegame.s"
.include "pda/texthook.s"
.include "pda/furfx.s"
.include "pda/texturedebug.s"
.include "pda/backpack.s"

# for proper memory alignment, this file must be included last.
.include "pda/vars.s"
