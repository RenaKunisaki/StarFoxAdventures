# PDA patch:
# Replaces the "PDA On/Off" item with a menu with lots of handy features.
# TODO: change the "PDA On/Off" text.
.text
.include "common.s"

# define patches
patchList:
    PATCH_ID "PDAMenu" # must be 7 chars
    PATCH_BL 0x80133A54, itemHook
    PATCH_MAIN_LOOP mainLoop
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set CUR_CHAR_ADDR,0x817FFFFF # for character swap
    .set MENU_WIDTH,300
    .set MENU_HEIGHT,200
    .set MENU_XPOS,320-(MENU_WIDTH/2)
    .set MENU_YPOS,240-(MENU_HEIGHT/2)
    .set LINE_HEIGHT,18
    .set STACK_SIZE,0x100 # how much to reserve
    .set SP_LR_SAVE,0x104 # this is what the game does
    .set SP_FLOAT_TMP,0x20 # temporary storage for float conversion (8 bytes)
    .set SP_GPR_SAVE,0x28

entry: # called as soon as our patch is loaded.
    # nothing to do
    blr


itemHook: # called when the PDA is being toggled on/off by player.
    # r13 = &pdaOn + 0x7630
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r5
    stw   r5, SP_LR_SAVE(r1)
    stmw  r3, SP_GPR_SAVE(r1)

    bl .itemHook_getpc
    .itemHook_getpc:
        mflr r29

    li   r3, 1
    stb  r3, (menuVisible - .itemHook_getpc)(r29)

    lwz  r5, SP_LR_SAVE(r1)
    mtlr r5 # restore LR
    lmw  r3, SP_GPR_SAVE(r1)
    addi r1, r1, STACK_SIZE # restore stack ptr

    JUMP 0x80133A94, r0 # skip the actual on/off code


# include various submodules.
# order doesn't really matter, but changing the order here will
# not affect the actual order displayed; for that, change the
# function pointers below.
.include "pda/main.s"
.include "pda/hud.s"
.include "pda/animation.s"
.include "pda/player.s"
.include "pda/pdahud.s"
# unfortunately this isn't safe.
# playing invalid sounds can crash the game.
#.include "pda/soundtest.s"
.include "pda/debugtext.s"
.include "pda/bigmap.s"
.include "pda/mapalpha.s"
.include "pda/fov.s"
.include "pda/gamespeed.s"
.include "pda/volume.s"

# some generic code used by multiple submodules.

adjItem_float: # r3=adj, r5=&val, f2=adjStep, f3=min, f4=max
    lfs     f1, 0(r5) # f1 = val
    cmpwi   r3, 0
    bge     .noNegFloat
    fneg    f2, f2
.noNegFloat:
    fadds   f1, f1, f2 # f2 = -adjStep
    fcmpo   0, f1, f3 # f1 < min?
    bge     .float_moreThanMin
    # XXX wraparound doesn't work nicely due to rounding error.
    # sometims you'll jump from 10% to 200% instead of to 0%.
    #fmr     f1, f4 # f1 = max
    fmr     f1, f3 # f1 = min
.float_moreThanMin:
    fcmpo   0, f1, f4 # f1 > max?
    ble     .float_lessThanMax
    #fmr     f1, f3 # f1 = min
    fmr     f1, f4 # f1 = max
.float_lessThanMax:
    stfs    f1, 0(r5)
    blr

# pointers to the submodules.
# ENSURE THAT THESE TABLES MATCH.
# each draw function must have a corresponding adjust function,
# and they must be in the same order.
# (I could probably combine the tables into one alternating, but I
# think that would be slightly less efficient as I wouldn't be able
# to use lwzu to index them.)
itemDrawFuncs:
    .int drawItem_player    - mainLoop
    .int drawItem_PDAHUD    - mainLoop
    .int drawItem_bigMap    - mainLoop
    .int drawItem_mapAlpha  - mainLoop
    #.int drawItem_sound     - mainLoop
    .int drawItem_FOV       - mainLoop
    .int drawItem_volMusic  - mainLoop
    .int drawItem_volSFX    - mainLoop
    .int drawItem_gameSpeed - mainLoop
    .int drawItem_debugText - mainLoop
    # this table must end with a null entry to mark the end of the menu.
    # other tables don't need a null entry.
    .int 0

itemAdjustFuncs:
    .int adjItem_player    - mainLoop
    .int adjItem_PDAHUD    - mainLoop
    .int adjItem_bigMap    - mainLoop
    .int adjItem_mapAlpha  - mainLoop
    #.int adjItem_sound     - mainLoop
    .int adjItem_FOV       - mainLoop
    .int adjItem_volMusic  - mainLoop
    .int adjItem_volSFX    - mainLoop
    .int adjItem_gameSpeed - mainLoop
    .int adjItem_debugText - mainLoop

# Variables and constant pools go here.
# To optimize memory layout, these should be in order from largest
# to smallest data types (ie int/float/pointer, short, byte/string).
# That way we avoid the need to insert padding to align the larger
# types properly.
# int/float/pointer constants can also be stuck at the end of the
# submodule files, since code, int, float, and pointer are all
# aligned to 4 bytes anyway.
# But, keeping them here can help to reduce memory usage slightly,
# because it makes it easier to see when a constant is duplicated
# and just reuse it for both labels.

# Since the GameText engine doesn't really allow for arbitrary box
# styles/positions, we steal one of the boxes for our menu.
# This points to the box in question.
boxAddr: .int 0x802c7588

fovOpDefault: .int 0xD02D96C4 # opcode to undo FOV patch

# menu animation
menuAnimTimer: .float 0
menuAnimSpeed: .float 0.125 # 1 / 8
#menuAnimSpeed: .float 0.066666667 # 1 / 15
#menuAnimSpeed: .float 0.0166666667 # 1 / 60

# float constants
zero:          .float   0
one:           .float   1
two:           .float   2
five:          .float   5
sixty:         .float  60
oneHundred:    .float 100
twoFiveFive:   .float 255
fovMax:        .float 175 # the most the game will do
gameSpeedMin:  .float  15 # 1/4 * 60; also the amount to adjust by
gameSpeedMax:  .float 240 # 4 * 60
gameSpeedDiv:  .float 0.6 # 60 / 100, to convert to percent
volAdjStep:    .float 0.1
floatMagic:    .int 0x43300000,0x80000000
# lol of course we can't do this.
#f_menuWidth:   .float MENU_WIDTH
#f_menuHeight:  .float MENU_HEIGHT
#f_menuXPos:    .float MENU_XPOS
#f_menuYPos:    .float MENU_YPOS
f_menuWidth:   .float 300
f_menuHeight:  .float 200
f_centerX:     .float 320
f_centerY:     .float 240

# menu state
minimapSizeOverride:  .short 0
minimapAlphaOverride: .byte 255
menuVisible:   .byte 0
menuSelItem:   .byte 0
menuSelColor:  .byte 0
menuJustMoved: .byte 0
#soundTestId:   .short 0

# string pool
title:       .string "PDA Menu"
s_on:        .string "On"
s_off:       .string "Off"
s_Player:    .string "Player: %s"
s_Fox:       .string "Fox"
s_Krystal:   .string "Krystal"
s_PDAHUD:    .string "PDA HUD: %s"
s_map:       .string "Map"
s_compass:   .string "Fuel Cell Compass"
s_info:      .string "Information"
#s_sound:     .string "Sound Test: %04X"
s_DebugText: .string "Debug Text: %s"
s_FOV:       .string "FOV: %d"
s_gameSpeed: .string "Game Speed: %d%%"
s_MapSize:   .string "Map Size: %s"
s_Normal:    .string "Normal"
s_Huge:      .string "Huge"
s_MapAlpha:  .string "Map Opacity: %d"
s_volMusic:  .string "Music Volume: %d%%"
s_volSFX:    .string "SFX Volume: %d%%"
s_volCS:     .string "CutScene Volume: %d%%"

strBuf: # buffer to print strings into
    .rept 64
    .byte 0
    .endr
