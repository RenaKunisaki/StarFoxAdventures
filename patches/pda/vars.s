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
s_MapAlpha:  .string "Map Opacity: %d%%"
s_volMusic:  .string "Music Volume: %d%%"
s_volSFX:    .string "SFX Volume: %d%%"
s_volCS:     .string "CutScene Volume: %d%%"
