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
f_menuWidth:   .float 320
f_menuHeight:  .float 220
f_centerX:     .float 320
f_centerY:     .float 240

# menu state
heapMenuIdx:   .short 0
bitMenuIdx:    .short 0
streamTestId:  .short 0
soundTestId:   .short 0
minimapSizeOverride:  .short 0
minimapAlphaOverride: .byte 255
menuVisible:   .byte 0
menuSelItem:   .byte 0
menuSelColor:  .byte 0
menuJustMoved: .byte 0 # frame counter
menuPage:      .byte 0
whichMenu:     .byte MENU_ID_MAIN
musicTestId:   .byte 0
warpMenuIdx:   .byte 0
whichHeap:     .byte 0
menuWasHudHidden: .byte 0
menuWasPdaOn:  .byte 0 # for hiding map in objects list

# string pool
# general
s_on:         .string "On"
s_off:        .string "Off"

# page 0
title_page0:  .string "PDA Menu"
s_PDAHUD:     .string "PDA HUD: %s"
s_map:        .string "Map"
s_compass:    .string "Fuel Cell Compass"
s_info:       .string "Information"
s_MapSize:    .string "Map Size: %s"
s_Normal:     .string "Normal"
s_Huge:       .string "Huge"
s_MapAlpha:   .string "Map Opacity: %d%%"

# page 1
title_page1:  .string "Game Settings"
s_Player:     .string "Player: %s"
s_Fox:        .string "Fox"
s_Krystal:    .string "Krystal"
s_FOV:        .string "FOV: %d"
s_gameSpeed:  .string "Game Speed: %d%%"

# page 2
title_page2:  .string "Audio"
s_volMusic:   .string "Music Volume: %d%%"
s_volSFX:     .string "SFX Volume: %d%%"
s_volCS:      .string "CutScene Volume: %d%%"
s_sound:      .string "Sound Test: %04X" # SFX don't have names.
s_music:      .string "Music Test: %02X %s"
s_stream:     .string "Stream Test: %04X %s"

# page 3
title_page3:  .string "Debug"
s_DebugText:  .string "Debug Text: %s"
s_TrickyDebug:.string "Tricky Debug Text: %s"
s_FreeMove:   .string "Free Move: %s"
s_ReloadMap:  .string "Reload Map"
s_Warp:       .string "Warp"
s_Objects:    .string "Objects"
s_GameBits:   .string "GameBits"
s_Heap:       .string "Heap"

# Objects menu
fmt_objListEntry:  .string "%04X %08X "
# right pane
fmt_objListCoords: .string "%08X  %d, %d, %d"
fmt_objListFlags:  .string "FLAG %04X %02X %02X"
fmt_objListMap:    .string "SLOT %02X MAP %02X %02X"
fmt_objListSeq:    .string "SEQ %08X %04X"
fmt_objListEvent:  .string "EVENT %08X"
fmt_objListModel:  .string "MODEL %08X(%d) %04X"
fmt_objListState:  .string "STATE %08X"
fmt_objListInstrs: .string "Z:Focus X:Delete"

# GameBits
fmt_bitListHeader: .string "BIdx    T Offs   Sz Unk: Val"
fmt_bitListEntry:  .string "%04X %X %04X %02X %03X: %X"

# Warps
fmt_warpListEntry:  .string "%02X %02X %s"
fmt_warpListCoords: .string "%f %f %f %d %02X"
fmt_warpListNoMap:  .string "-"
fmt_warpListInstrs: .string "A:Warp Z:Jump"

# Heap
fmt_heapListHeader: .string "N Bytes        Blocks  UsedBytes  UBlk  FreeBytes    FBlk Data"
fmt_heapListEntry:  .string "%d %08X %04X %08X %04X %08X %04X %08X"
fmt_heapMenuHeader: .string "Num   Tag               Data          Size"
fmt_heapMenuEntry:  .string "%04X %08X %08X %08X"

# 803dca3e bool shouldResetNextFrame
# not sure where to put this, and would want to add
# some kind of confirmation to it.
# plus it probably has soft reset hotkey already...
# it does, X+B+Start
# but maybe can do reset to title screen or reload save

# other things to add:
# - Arbitrary warp (set coords/layer manually)
#   - edit the WARPTAB enties in memory?
#   - Map grid view?
# - Browse loaded file list?
# - Browse GameText, textures, models, animations?
# - Memory editor
# - Spawn objects if we can figure out how
# - fun stuff
#   - bullet time (reduce game speed during combat)
#   - dynamic FOV (increase with player velocity)
#     - increase game speed with player velocity?
#     - or decrease so it's like you go the same speed but
#       the rest of the world slows down
#   - color filters
