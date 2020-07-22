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
f_m60:         .float  -60
zero:          .float    0
one:           .float    1
two:           .float    2
five:          .float    5
f_24:          .float   24
sixty:         .float   60
oneHundred:    .float  100
twoFiveFive:   .float  255
f_3600:        .float 3600
fovMax:        .float  175 # the most the game will do
gameSpeedMin:  .float   15 # 1/4 * 60; also the amount to adjust by
gameSpeedMax:  .float  240 # 4 * 60
gameSpeedDiv:  .float  0.6 # 60 / 100, to convert to percent
volAdjStep:    .float  0.1
# lol of course we can't do this.
#f_menuWidth:   .float MENU_WIDTH
#f_menuHeight:  .float MENU_HEIGHT
#f_menuXPos:    .float MENU_XPOS
#f_menuYPos:    .float MENU_YPOS
f_menuWidth:   .float 320
f_menuHeight:  .float 230
f_centerX:     .float 320
f_centerY:     .float 240
f_mapCellScale: .float 640.0

# menu state
heapMenuIdx:   .short 0
bitMenuIdx:    .short 0
streamTestId:  .short 0
soundTestId:   .short 0
textTestId:    .short 0
minimapSizeOverride:  .short 0
minimapAlphaOverride: .byte 255
hudFullScreen:    .byte 1
menuVisible:      .byte 0
menuSelItem:      .byte 0
menuSelColor:     .byte 0
menuJustMoved:    .byte 0 # frame counter
menuPage:         .byte 0
whichMenu:        .byte MENU_ID_MAIN
musicTestId:      .byte 0
warpMenuIdx:      .byte 0
saveMenuIdx:      .byte 0
saveMenuObjIdx:   .byte 0
whichHeap:        .byte 0
menuWasHudHidden: .byte 0
menuWasPdaOn:     .byte 0 # for hiding map in objects list
menuMapSize:      .byte 0

# string pool
# general
s_on:            .string "On"
s_off:           .string "Off"
fmt_04X:         .string "%04X"
fmt_08X:         .string "%08X"

# page 0
title_page0:  .string "PDA Menu"
s_PDAHUD:     .string "PDA HUD: %s"
s_map:        .string "Map"
s_compass:    .string "Fuel Cell Compass"
s_info:       .string "Information"
s_MapSize:    .string "Map Size: %s"
s_Normal:     .string "Normal"
s_Huge:       .string "Huge"
s_Small:      .string "Small"
s_MapAlpha:   .string "Map Opacity: %d%%"

# page 1
title_page1:  .string "Game Settings"
s_Player:     .string "Player: %s"
s_Fox:        .string "Fox"
s_Krystal:    .string "Krystal"
s_FOV:        .string "FOV: %d"
s_gameSpeed:  .string "Game Speed: %d%%"
s_autoSave:   .string "Autosave: %s"
.if 0
s_language:   .string "Language: %s"
s_English:    .string "English"
s_French:     .string "French"
s_German:     .string "German"
s_Italian:    .string "Italian"
s_Japanese:   .string "Japanese"
s_Spanish:    .string "Spanish"
.endif

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
s_DebugObjs:  .string "Debug Objects: %s"
s_FreeMove:   .string "Free Move: %s"
s_ReloadMap:  .string "Reload Map"
s_Warp:       .string "Warp"
s_Objects:    .string "Objects"
s_GameBits:   .string "GameBits"
s_SaveGame:   .string "SaveGame Data"
s_Heap:       .string "Heap"
s_TextTest:   .string "Show Text: %04X"
s_frameAdv:   .string "Advance Frame"

# Objects menu
fmt_objListEntry:  .string "%04X %08X "
# right pane
fmt_objListCoords: .string "POS  %d, %d, %d"
fmt_objListOrigPos:.string "ORIG %d, %d, %d"
fmt_objListAddrId: .string "ADDR   %08X ID %08X"
fmt_objListAddr:   .string "ADDR   %08X ID --"
fmt_objListNoSeq:  .string "NO SEQ"
fmt_objListFile:   .string "OBJDEF %08X"
fmt_objListFlags:  .string "FLAG   %04X %02X %02X %02X %02X"
fmt_objListMap:    .string "SLOT   %02X MAP %02X %02X"
fmt_objListSeq:    .string "SEQ    %08X %04X"
fmt_objListEvent:  .string "EVENT  %08X"
fmt_objListModel:  .string "MODEL  %08X(%d) %04X"
fmt_objListState:  .string "STATE  %08X"
fmt_objListHitbox: .string "HITBOX %08X"
fmt_objListFuncs:  .string "FUNCS  %08X"
fmt_objListParent: .string "PARENT %08X %s"
fmt_objListChild:  .string "CHILD  %08X %s"
fmt_objListInstrs1:.string "Z:Focus S:Player"
fmt_objListInstrs2:.string "Y:GoTo  X:Delete"
s_noObjs:          .string "No objects"

# GameBits
fmt_bitListHeader: .string "Bit  T Offs Sz Unk: Val"
fmt_bitListEntry:  .string "%04X %X %04X %02X %03X: %X"

# Warps
fmt_warpListEntry:  .string "%02X %02X %s"
fmt_warpListCoords: .string "%f %f %f %d %02X"
fmt_warpListNoMap:  .string "-"
#fmt_warpListInstrs: .string "A:Warp Z:Jump"
#fmt_warpListInstrs: .string "A:Warp"

# Heap
fmt_heapListHeader: .string "N Bytes    Blox UsedByts UBlk FreeByts FBlk Data"
fmt_heapListEntry:  .string "%d %08X %04X %08X %04X %08X %04X %08X"
fmt_heapMenuHeader: .string "Num  Tag         Data       Size"
#fmt_heapMenuEntry:  .string "%04X %08X %08X %08X"

# SaveGame
s_savegameEmpty:        .string "NO DATA"
s_savegameCurChr:       .string "Chr: %s; SaveName: %s"
s_savegamePlayTime:     .string "%dd %02dh %02dm %02ds %02df"
s_savegameItem_Game:    .string "Game    State"
s_savegameItem_Krystal: .string "Krystal State"
s_savegameItem_Fox:     .string "Fox     State"
s_savegameItem_Objs:    .string "Saved Objects"
s_savegameHPMP:         .string "HP %02d/%02d  MP %02d/%02d"
s_savegameLivesMoney:   .string "1up %d/%d  Money: %d"
s_savegameUnkFlags:     #.string "Unk: %02X %02X %02X"
s_savegameUnkChrState:  .string "Unk %02X %02X %02X"
s_savegameCoordX:       .string "X %f"
s_savegameCoordY:       .string "Y %f"
s_savegameCoordZ:       .string "Z %f"
s_savegameLayer:        .string "Rot %d Grid %d unk %04X"
s_savegameMap:          .string "Map: %s"
s_savegameSavedObj:     .string "%08X %d %d %d"


# 803dca3e bool shouldResetNextFrame
# not sure where to put this, and would want to add
# some kind of confirmation to it.
# plus it probably has soft reset hotkey already...
# it does, X+B+Start
# but maybe can do reset to title screen or reload save
# - we can reset to title screen from pause menu.

# other things to add:
# - Arbitrary warp (set coords/layer manually)
#   - edit the WARPTAB enties in memory?
#   - Map grid view?
# - Browse loaded file list, textures, models, animations?
# - Memory editor
# - Spawn objects if we can figure out how
# - fun stuff
#   - bullet time (reduce game speed during combat)
#   - dynamic FOV (increase with player velocity)
#     - increase game speed with player velocity?
#     - or decrease so it's like you go the same speed but
#       the rest of the world slows down
#   - color filters
