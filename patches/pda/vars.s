.ascii "pda/vars" # 8 byte file ID for debug
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

.set SPAWN_MENU_MAX_PARAMS,10
.align 4 # just in case

# float constants
f_m60:         .float  -60
zero:          .float    0
one:           .float    1
two:           .float    2
five:          .float    5
f_24:          .float   24
f_60:          .float   60
oneHundred:    .float  100
f_127:         .float  127
twoFiveFive:   .float  255
f_3600:        .float 3600
fovMax:        .float  175 # the most the game will do
gameSpeedMin:  .float   15 # 1/4 * 60; also the amount to adjust by
gameSpeedMax:  .float  240 # 4 * 60
gameSpeedDiv:  .float  0.6 # 60 / 100, to convert to percent
volAdjStep:    .float  0.01
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
spawnMenuObjId: .int -1
spawnMenuParams:
    .rept SPAWN_MENU_MAX_PARAMS
    .int  0
    .endr
spawnMenuXPos:  .int 0
spawnMenuYPos:  .int 0
spawnMenuZPos:  .int 0
spawnMenuType:  .short 0x2CF
heapMenuIdx:    .short 0
bitMenuIdx:     .short 0
streamTestId:   .short 0
soundTestId:    .short 0
textTestId:     .short 0
textTestBox:    .short 0x82
textureDebugId: .short 0
debugSeqId:     .short 0
minimapSizeOverride:  .short 0
minimapAlphaOverride: .byte 255
bitMenuCursorX:       .byte 7
spawnMenuNumParams:   .byte 0
spawnMenuCursor:      .byte 0
spawnMenuCursorX:     .byte 0
spawnMenuFlags:       .byte 5
spawnMenuLoadFlags:   .byte 0x8
spawnMenuMapStates1:  .byte 0
spawnMenuMapStates2:  .byte 4
spawnMenuBound:       .byte 100
spawnMenuUnk7:        .byte 0x96
spawnMenuMapId:       .byte 0xFF
spawnMenuObjNo:       .byte 0xFF
dbgAudioCmd:      .byte 0
dbgAudioParam:    .byte 0
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
textureDebugMip:  .byte 0
debugSeqIdx:      .byte 0

# string pool
# general
emptyStr:         .byte 0
s_on:             .string "On"
s_off:            .string "Off"
fmt_04X:          .string "%04X"
fmt_08X:          .string "%08X"
s_Normal:         .string "Normal"
s_invert:         .string "Invert"
s_Huge:           .string "Huge"
s_Small:          .string "Small"
s_Always:         .string "Always"
s_pad1:           .string "Controller 1"
s_pad3:           .string "Controller 3"
s_Fox:            .string "Fox"
s_Krystal:        .string "Krystal"
s_xDec:           .string "X: %d"
s_yDec:           .string "Y: %d"


# page 0
s_GameSettings:   .string "Game Settings"
s_Player:         .string "Player: %s"
s_gameSpeed:      .string "Game Speed: %d%%"
s_autoSave:       .string "Autosave: %s"
.if 0
s_language:       .string "Language: %s"
s_English:        .string "English"
s_French:         .string "French"
s_German:         .string "German"
s_Italian:        .string "Italian"
s_Japanese:       .string "Japanese"
s_Spanish:        .string "Spanish"
.endif
s_subtitles:      .string "Subtitles: %s"
s_rumble:         .string "Rumble: %s"
s_rumbleBlur:     .string "Rumble Blur: %s"
s_backpack:       .string "Backpack: %s"

# page 1
s_VideoSettings:  .string "Video Settings"
s_PDAHUD:         .string "PDA HUD: %s"
s_map:            .string "Map"
s_compass:        .string "Fuel Cell Compass"
s_info:           .string "Information"
s_MapSize:        .string "Map Size: %s"
s_MapAlpha:       .string "Map Opacity: %d%%"
s_MapZoom:        .string "Map Zoom: %d%%"
s_FurEffect:      .string "Fur Effect: %s"

# page 2
s_CameraSettings: .string "Camera Settings"
s_FOV:            .string "FOV: %d"
s_LOD:            .string "LOD: %d"
s_widescreen:     .string "Widescreen: %s"
s_camPad:         .string "Camera Control: %s"
s_camInvX:        .string "Camera X: %s"
s_camInvY:        .string "Camera Y: %s"

# page 3
s_AudioSettings:  .string "Audio Settings"
s_volMusic:       .string "Music Volume: %d%%"
s_volSFX:         .string "SFX Volume: %d%%"
s_volCS:          .string "CutScene Volume: %d%%"
s_soundMode:      .string "Sound Mode: %s"
s_music:          .string "Music Test: %02X %s"
s_sound:          .string "Sound Test: %04X" # SFX don't have names.
s_stream:         .string "Stream Test: %04X %s"
s_stereo:         .string "Stereo"
s_surround:       .string "Surround"
s_mono:           .string "Mono"
s_headphones:     .string "Headphones"

# page 4
s_Debug:          .string "Debug"
s_DebugText:      .string "Debug Text"
s_Map:            .string "Map"
s_Data:           .string "Data"
s_Render:         .string "Render"
s_frameAdv:       .string "Advance Frame"

# Debug Text menu
#title_page5:      .string "Debug Text"
s_Master:         .string "Master: %s"
s_Tricky:         .string "Tricky: %s"
s_PlayerState:    .string "Player State: %s"
s_SeqState:       .string "Seq State: %s"
s_HeapState:      .string "Heaps: %s"
s_RestartPoint:   .string "Restart Point: %s"

# Debug Map menu
#title_page7:      .string "Map"
s_FreeMove:       .string "Free Move: %s"
s_ReloadMap:      .string "Reload Map"
s_Warp:           .string "Warp"
s_DebugObjs:      .string "Debug Objects: %s"
s_ObjList:        .string "Object List"
s_SetRestartPoint:  .string "Set Restart Point"
s_ClrRestartPoint:  .string "Clear Restart Point"
s_GoToRestartPoint: .string "Goto Restart Point"

# Debug Data menu
#title_page8:        .string "Data"
s_GameBits:         .string "GameBits"
s_SaveGame:         .string "SaveGame Data"
s_Heap:             .string "Heap"

# Debug Render menu
s_Textures:       .string "Textures"
s_Hitboxes:       .string "Hitboxes: %s"
s_BlurFilter:     .string "Blur Filter: %s"
s_MotionBlur:     .string "Motion Blur: %3d"
s_HeatEffect:     .string "Heat Effect: %3d"
s_Monochrome:     .string "Monochrome: %s"
s_SpiritVision:   .string "Spirit Vision: %s"
#s_Distortion:     .string "Distort Filter: %s"
s_ColorFilter:    .string "Color Filter: %s"
s_Red:            .string "Red: %3d"
s_Green:          .string "Green: %3d"
s_Blue:           .string "Blue: %3d"
s_Alpha:          .string "Alpha: %3d"

# Debug Cheat menu
s_EditPlayerState:.string "Edit Player State"
s_curHP:          .string "Cur HP: %d"
s_maxHP:          .string "Max HP: %d"
s_curMP:          .string "Cur MP: %d"
s_maxMP:          .string "Max MP: %d"
s_curMoney:       .string "Money: %d"
s_curLives:       .string "Cur Lives: %d"
s_maxLives:       .string "Max Lives: %d"
s_unlockAll:      .string "Unlock Everything"
s_killMe:         .string "Kill Player"

# Debug Misc menu
s_Misc:           .string "Misc"
s_CrashGame:      .string "Crash Game"

# Debug Camera menu
s_Camera:         .string "Camera"
s_Mode:           .string "Mode: %s"
s_Stay:           .string "Stay"
s_Free:           .string "Free"
#s_InterpMode:     .string "Interp Mode: %d"

# Debug Environment menu
s_Environment:     .string "Environment"
s_timeOfDay:       .string "Time: %02d:%02d:%02d"
s_timeOfDayNA:     .string "Time: N/A"

# Debug GameText menu
s_GameText:        .string "GameText"
s_TextTest:        .string "Text: %04X"
s_TextBox:         .string "Window: %04X"

# Debug Audio menu
s_Audio:           .string "Audio"
s_DbgAudioCmd:     .string "Command: %02X %d [Z]"

# Debug Sequence menu
s_ObjSeq:         .string "ObjSeq"
s_PlaySeq:        .string "Play Seq %04X"
s_StopSeq:        .string "Stop Seq"
s_EditSeq:        .string "Edit Seq %02X"
s_GlobalN:        .string "Global %d: %04X"
s_SeqBool:        .string "Bool: %d"
s_SeqVar:         .string "Var %d: %02X"
s_SeqFlags:       .string "Flags: %02X"

# Objects menu
fmt_objListEntry:  .string "%04X %08X "
# right pane
fmt_objListCoords:   .string "POS  %d, %d, %d"
fmt_objListOrigPos:  .string "ORIG %d, %d, %d"
fmt_objListAddrId:   .string "ADDR   %08X ID %08X"
fmt_objListAddr:     .string "ADDR   %08X ID --"
fmt_objListNoSeq:    .string "NO SEQ"
fmt_objListFile:     .string "FILE   %08X DEFNO %04X"
fmt_objListCat:      .string "CATID  %04X OBJNO %04X"
fmt_objListFlags:    .string "FLAG   %04X %02X %02X %02X %02X"
fmt_objListMap:      .string "SLOT   %02X MAP %02X %02X"
fmt_objListSeq:      .string "SEQ    %08X %04X"
fmt_objListEvent:    .string "EVENT  %08X"
fmt_objListModel:    .string "MODEL  %08X(%d) %04X"
fmt_objListState:    .string "STATE  %08X"
fmt_objListHitbox:   .string "HITBOX %08X"
fmt_objListFuncs:    .string "FUNCS  %08X"
fmt_objListMsgQueue: .string "MSGS   %08X %d/%d"
fmt_objListParent:   .string "PARENT %08X %s"
fmt_objListChild:    .string "CHILD  %08X %s"
fmt_objListInstrs1:  .string "Z:Focus S:Player"
fmt_objListInstrs2:  .string "Y:GoTo  X:Delete"
s_noObjs:            .string "No objects"

# Spawn menu
s_spawn:           .string "Spawn Object"
s_spawnType:       .string "Object:      %04X (%04X) %s"
s_spawnNumParams:  .string "Params:      %d"
s_spawnFlags:      .string "Spawn Flags: %s %02X"
s_loadFlags:       .string "Load  Flags: %s %02X"
s_mapStates1:      .string "Map States1: %s %02X"
s_mapStates2:      .string "Map States2: %s %02X"
s_bound:           .string "Bounds:      %s %02X"
s_unk7:            .string "Unk07:       %s %02X"
s_objID:           .string "Obj ID:      %08X"
s_spawnMap:        .string "Map ID:      %s %02X"
s_spawnObjNo:      .string "Obj No.:     %s %02X"
s_xPos:            .string "X Position:  %08X"
s_yPos:            .string "Y Position:  %08X"
s_zPos:            .string "Z Position:  %08X"
s_spawnParam:      .string "Param %02X:    %08X"
s_spawnInstrs:     .string "Start:Spawn B:Exit X:+ Y:- Z:Player Coords"
s_spawned:         .string "Spawned obj: %08X @ %f, %f, %f"

# GameBits
fmt_bitListHeader: .string  "Bit  T Value    Text"
fmt_bitListEntry:  .string "%04X %X %08X %s"
fmt_bitListInstrs: .string "B:Exit X:+ Y:- L/R:Page"

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

# Textures
s_textureId:            .string "Tex %04X.%02X: ID %c%04X @%08X"
s_textureInfo:          .string "Flags: %08X RefCnt: %3d Fmt: %02X; %d x %d"

# Warps
warpNameHollowOutside: .string "ThornTail Hollow - Outside"
warpNameEggCave:       .string "ThornTail Hollow - Egg Cave"
warpNameHollowBottom:  .string "ThornTail Hollow - Bottom"
warpNameShop:          .string "ThornTail Store"
warpNameKPstart:       .string "Krazoa Palace - Starting Area"
warpNameKPwarpstone:   .string "Krazoa Palace - WarpStone Pad"
warpNameKPinside:      .string "Krazoa Palace - Inside"
warpNameKPfinalboss:   .string "Krazoa Palace - Final Boss Warp"
warpNameKPtop:         .string "Krazoa Palace - Top"
warpNameIMtop:         .string "Ice Mountain - Top"
warpNameSHwarp:        .string "SnowHorn Wastes - Shrine Warp"
warpNameSHspout:       .string "SnowHorn Wastes - Water Spout"
warpNameSHent:         .string "SnowHorn Wastes - Entrance"
warpNameSHbot:         .string "SnowHorn Wastes - Bottom"
warpNameDIMpad:        .string "DarkIce Mines - Landing Pad"
warpNameDIMfort:       .string "DarkIce Mines - SharpClaw Fort"
warpNameDIMbike:       .string "DarkIce Mines - Bike Start"
warpNameDIMinterior:   .string "DarkIce Mines - Interior"
warpNameDIMice:        .string "DarkIce Mines - Ice Section"
warpNameDIMbot:        .string "DarkIce Mines - Bottom"
warpNameMMPhut:        .string "Moon Mountain Pass - Hut"
warpNameMMPwarp:       .string "Moon Mountain Pass - Top"
warpNameMMPvolcano:    .string "Moon Mountain Pass - Volcano Entrance"
warpNameMMPbot:        .string "Moon Mountain Pass - Bottom"
warpNameLVshrine:      .string "LightFoot Village - Shrine Warp"
warpNameLVfront:       .string "LightFoot Village - Front"
warpNameLVback:        .string "LightFoot Village - Back"
warpNameCC:            .string "Cape Claw"
warpNameCFdung:        .string "CloudRunner Fortress - Dungeon"
warpNameCFentrance:    .string "CloudRunner Fortress - Entrance"
warpNameCFqueen:       .string "CloudRunner Fortress - Queen Cage"
warpNameCRrace:        .string "CloudRunner Race Start"
warpNameCRmine:        .string "CloudRunner Mine"
warpNameOFPbot:        .string "Ocean Force Point - Bottom"
warpNameOFPend:        .string "Ocean Force Point - End"
warpNameOFPplat:       .string "Ocean Force Point - SpellStone Platform"
warpNameOFPentr:       .string "Ocean Force Point - Entrance"
warpNameOFPtop:        .string "Ocean Force Point - Top Warp"
warpNameOFPbot2:       .string "Ocean Force Point - Bottom Warp"
warpNameVFPentrance:   .string "Volcano Force Point - Entrance"
warpNameVFPcave:       .string "Volcano Force Point - Cave Entrance"
warpNameVFPplat:       .string "Volcano Force Point - SpellStone Platform"
warpNameVFPstone:      .string "Volcano Force Point - SpellStone Platform 2"
warpNameVFPwarp:       .string "Volcano Force Point - End Warp"
warpNameWCpyramid:     .string "Walled City - Front of Pyramid"
warpNameWCboss:        .string "Walled City - Boss Door"
warpNameDragRock:      .string "Dragon Rock - Landing Pad"
warpNameDragRockBird:  .string "Dragon Rock - CloudRunner Cage"
warpNameTestCombat:    .string "Test of Combat"
warpNameTestStrength:  .string "Test of Strength"
warpNameTestKnowledge: .string "Test of Knowledge"
warpNameTestFear:      .string "Test of Fear"
warpNameTestSkill:     .string "Test of Skill"
warpNameGaldon:        .string "Boss: Galdon"
warpNameGaldonInside:  .string "Boss: Galdon - Inside"
warpNameDrakor:        .string "Boss: Drakor"
warpNameTrex:          .string "Boss: T-Rex"
warpNameScales:        .string "Boss: General Scales"
warpNameAndross:       .string "Boss: Andross"
warpNameWorldMap:      .string "World Map"
warpNameARWplanet:     .string "Arwing: to Dinosaur Planet"
warpNameARWdarkice:    .string "Arwing: to DarkIce Mines"
warpNameARWcloud:      .string "Arwing: to CloudRunner Fortress"
warpNameARWcity:       .string "Arwing: to Walled City"
warpNameARWrock:       .string "Arwing: to Dragon Rock"
warpNameMazeCave:      .string "Game Well Maze"
warpNameMagicCave:     .string "Magic Cave"
warpNameGreatFox:      .string "Cutscene: Great Fox"
warpNameTitle:         .string "Cutscene: Title Screen"
warpNameShipBattle:    .string "Ship Battle"
warpNameAnimTest:      .string "Unused: AnimTest"
warpNameWGshrine:      .string "Unused: WGShrine"
#warpNameNikTest:       .string "Unused: NikTest"
warpNameRollDemo:      .string "Unused: Rolling Demo"
warpNameKamerian:      .string "Unused: Kamerian"
warpNameDuster:        .string "Unused: Duster Cave"
warpNameDiamondBay:    .string "Unused: Diamond Bay"
warpNameDiscovery:     .string "Unused: Discovery Falls"
warpNameWillow:        .string "Unused: Willow Grove"
warpNameOldKP:         .string "Unused: Old Krazoa Palace"
warpNameCloudRace2:    .string "Unused: CloudRunner Race 2"


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
