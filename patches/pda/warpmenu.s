.set WARP_MENU_XPOS,   20
.set WARP_MENU_YPOS,   56
.set WARP_MENU_WIDTH, 600
.set WARP_MENU_HEIGHT,300
.set MAX_WARP,         75

warpMenu:
    # subroutine: runs the Warp menu.
    # expects r14 = mainLoop.
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    #bl      menuHideHud
    #bl      menuSetFixedWidth
    bl      warpMenu_doInput
    bl      warpMenu_Main
    b       menuEndSub


warpMenu_Main: # draw list of warps.
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    # draw the box
    li      r3, WARP_MENU_XPOS   # X
    li      r4, WARP_MENU_YPOS   # Y
    li      r5, WARP_MENU_WIDTH  # width
    li      r6, WARP_MENU_HEIGHT # height
    li      r20, 255 # opacity
    bl      menuDrawBox

    # first item (selected) in blue
    LOAD    r3, 0x00FFFFFF
    bl      menuSetTextColor

    lbz     r17, (warpMenuIdx - mainLoop)(r14)
    li      r20, WARP_MENU_YPOS + 8 # string Y pos
    addi    r15, r14, warpDefs - mainLoop

.warpMenu_nextWarp:
    slwi    r9,  r17, 4
    add     r9,  r9, r15  # r4 = warp entry*
    lha     r5,  0x0E(r9) # name offset
    add     r3,  r5,  r14
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  WARP_MENU_XPOS + 8  # X pos
    mr      r6,  r20 # Y pos
    CALL    gameTextShowStr

    LOAD    r3, 0xFFFFFFFF
    bl      menuSetTextColor

    # next line
    addi    r17, r17, 1
    cmpwi   r17, MAX_WARP # last entry?
    bge     menuEndSub
    addi    r20, r20, LINE_HEIGHT
    cmpwi   r20, WARP_MENU_YPOS + WARP_MENU_HEIGHT - LINE_HEIGHT
    blt     .warpMenu_nextWarp
    b       menuEndSub


warpMenu_doInput:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)
    lbz     r17, (warpMenuIdx - mainLoop)(r14)

    # check the buttons
    bl      menuGetInput
    # r3=buttons, r4=stick X, r5=stick Y,
    # r6=CX, r7=CY, r8=L, r9=R
    andi.   r10, r3,  PAD_BUTTON_B
    bne     .warpMenu_close
    andi.   r10, r3,  PAD_BUTTON_A
    bne     .warpMenu_doWarp
    andi.   r10, r3,  PAD_BUTTON_Z
    bne     .warpMenu_doSwap

    # check analog stick
    cmpwi   r5,  0x10
    bgt     .warpMenu_up
    cmpwi   r5, -0x10
    blt     .warpMenu_down

    # check C stick - same as analog but no delay
    cmpwi   r7,  0x10
    bgt     .warpMenu_up
    cmpwi   r7, -0x10
    blt     .warpMenu_down

    # check L/R - jump by page
    cmpwi   r8,  0x04
    bgt     .warpMenu_prevPage
    cmpwi   r9,  0x04
    bgt     .warpMenu_nextPage

    b       menuEndSub

.warpMenu_up: # up pressed
    subi    r17, r17, 1
.warpMenu_up2:
    cmpwi   r17, 0
    bge     .warpMenu_storeIdx
    addi    r17, r17, MAX_WARP - 1

.warpMenu_down: # down pressed
    addi    r17, r17, 1
.warpMenu_down2:
    cmpwi   r17, MAX_WARP
    blt     .warpMenu_storeIdx
    li      r17, 0

.warpMenu_storeIdx:
    stb     r17, (warpMenuIdx - mainLoop)(r14)
    # don't use any delay if using C stick.
    andi.   r7,  r7,  0xF0 # deadzone
    bne     menuEndSub
    li      r3,  MOVE_DELAY
    stb     r3,  (menuJustMoved - mainLoop)(r14)
    b       menuEndSub

.warpMenu_prevPage:
    subi    r17, r17, 0x10
    b       .warpMenu_up2

.warpMenu_nextPage:
    addi    r17, r17, 0x10
    b       .warpMenu_down2

.warpMenu_close:
    bl      returnToMainMenu
    b       menuEndSub

.warpMenu_doWarp:
    #CALL    0x80056f7c # unloadMap
    addi    r15, r14, warpDefs - mainLoop
    LOAD    r18, 0x803a32a8 # saveData.curSaveGame
    lbz     r3,  0x20(r18) # character
    slwi    r3,  r3,  4 # times 0x10, size of PlayerCharPos
    addi    r3,  r3,  0x684
    add     r3,  r3,  r18

    # calling warpToMap doesn't initialize the map correctly.
    # it's meant to be used from a sequence.
    # instead, we'll change the player's "last saved" position and then
    # load that as if we were loading the save file.
    lbz     r17, (warpMenuIdx - mainLoop)(r14)
    slwi    r9,  r17, 4
    add     r9,  r9, r15 # r4 = warp entry*
    lfs     f4,  (f_mapCellScale - mainLoop)(r14)
    lfs     f1,  0x00(r9) # X
    lfs     f2,  0x04(r9) # Y
    lfs     f3,  0x08(r9) # Z
    stfs    f1,  0x00(r3)
    stfs    f2,  0x04(r3)
    stfs    f3,  0x08(r3)
    lbz     r4,  0x0C(r9) # layer
    stb     r4,  0x0D(r3)
    CALL    0x800e9be0    # loadMapForCurrentSaveGame
    b       .warpMenu_close

.warpMenu_doSwap:
    LOAD    r18, 0x803a32a8 # saveData.curSaveGame
    lbz     r3,  0x20(r18) # character
    xori    r3,  r3,  1
    stb     r3,  0x20(r18)
    b       .warpMenu_doWarp

.macro DEFINEWARP x, y, z, layer, name
    .float \x
    .float \y
    .float \z
    .byte  \layer
    .byte  0 # padding
    .short \name - mainLoop
.endm

warpDefs:
    DEFINEWARP  -5774,   -765,  -2542,  0, warpNameHollowOutside
    DEFINEWARP  -5517,   -636,   -800,  0, warpNameEggCave
    DEFINEWARP  -6664,  -1086,  -2253, -1, warpNameHollowBottom
    DEFINEWARP  -6069,   -980,  -2752,  1, warpNameShop
    DEFINEWARP  12264,    211,    451,  0, warpNameKPstart
    DEFINEWARP  11840,   -245,   5864,  0, warpNameKPwarpstone
    DEFINEWARP  13154,    -83,   2880,  0, warpNameKPinside
    DEFINEWARP  13201,   1278,   2239,  0, warpNameKPfinalboss
    DEFINEWARP  12188,   1379,   2461,  0, warpNameKPtop
    DEFINEWARP   3336,   6484,   4183,  0, warpNameIMtop
    DEFINEWARP  -4159,   -521,   4159,  0, warpNameSHwarp
    DEFINEWARP  -2634,   -130,   2167,  0, warpNameSHspout
    DEFINEWARP  -5408,   -822,   1610,  0, warpNameSHent
    DEFINEWARP   -511,    -41,   2951,  0, warpNameSHbot
    DEFINEWARP  -7378,  -1557,   8935,  0, warpNameDIMpad
    DEFINEWARP  -7957,  -1177,  14025,  0, warpNameDIMfort
    DEFINEWARP -11923,   -268,   -331,  0, warpNameMMPhut
    DEFINEWARP -11839,     52,  -4791,  0, warpNameMMPwarp
    DEFINEWARP -13440,   -111,   -381,  0, warpNameMMPvolcano
    DEFINEWARP  -8902,   -925,   -313,  0, warpNameMMPbot
    DEFINEWARP  -1601,  -1216,   -960,  0, warpNameLVshrine
    DEFINEWARP  -1299,   -814,  -1932,  0, warpNameLVfront
    DEFINEWARP  -3150,   -895,  -1758,  0, warpNameLVback
    DEFINEWARP   1845,  -1624,  -2330,  0, warpNameCC
    DEFINEWARP    829,   1302, -16477,  0, warpNameCFdung
    DEFINEWARP   4125,   1464, -18232,  0, warpNameCFentrance
    DEFINEWARP    233,   2196, -17591,  0, warpNameCFqueen
    DEFINEWARP   -173,   1284, -14400,  0, warpNameCRrace
    DEFINEWARP    463,   1845, -16899,  0, warpNameCRmine
    DEFINEWARP   1534,  -2536,  -8574, -1, warpNameOFPbot
    DEFINEWARP   4220,  -2536,  -8576, -1, warpNameOFPend
    DEFINEWARP   3198,  -2542,  -7981, -1, warpNameOFPplat
    DEFINEWARP   3357,  -1600,  -7428,  0, warpNameOFPentr
    DEFINEWARP   2872,  -1533,  -9247,  0, warpNameOFPtop
    DEFINEWARP   2877,  -2718,  -9277, -1, warpNameOFPbot2
    DEFINEWARP -16065,     57,   -320,  0, warpNameVFPentrance
    DEFINEWARP -18710,    389,   -307,  0, warpNameVFPcave
    DEFINEWARP -17919,    198,   -975,  0, warpNameVFPplat
    DEFINEWARP -18648,    -45,    499,  0, warpNameVFPstone
    DEFINEWARP -18564,    293,    647,  0, warpNameVFPwarp
    DEFINEWARP -16322,   -777, -13271,  0, warpNameWCpyramid
    DEFINEWARP -16322,  -1209, -13518,  0, warpNameWCboss
    DEFINEWARP -13753,  -1714,  11227,  0, warpNameDragRock
    DEFINEWARP  14079,   -204, -17065,  0, warpNameTestCombat
    DEFINEWARP  17919,   -204, -10669,  0, warpNameTestStrength
    DEFINEWARP  17919,   -204, -14511,  0, warpNameTestKnowledge
    DEFINEWARP  10240,   -203, -18354,  0, warpNameTestFear
    DEFINEWARP  13440,   -204, -13225,  0, warpNameTestSkill
    DEFINEWARP  -9555,  -2973,  20457, -2, warpNameGaldon
    DEFINEWARP  -7954,  -2978,  20162, -2, warpNameGaldonInside
    DEFINEWARP   7694,   -199,  25943,  1, warpNameDrakor
    DEFINEWARP -10138,      0, -13381,  0, warpNameTrex
    DEFINEWARP   7039,     58, -13887,  0, warpNameScales
    DEFINEWARP -11850,    131, -15476,  2, warpNameAndross
    DEFINEWARP   9628,    168,  -9566,  0, warpNameWorldMap
    DEFINEWARP -17280,    168, -17280,  2, warpNameARWplanet
    DEFINEWARP  -5120,    168, -17280,  2, warpNameARWdarkice
    DEFINEWARP   6400,    168, -17280,  2, warpNameARWcloud
    DEFINEWARP  10240,    168, -17280,  2, warpNameARWcity
    DEFINEWARP  14080,    168, -17280,  2, warpNameARWrock
    DEFINEWARP   7203,     86,  30980,  0, warpNameMazeCave
    DEFINEWARP   9600,    300, -15360,  0, warpNameMagicCave
    DEFINEWARP   -961,      9,  24522,  0, warpNameGreatFox
    DEFINEWARP -18870,      9,  28360,  0, warpNameTitle
    DEFINEWARP      0,      0,  13440,  0, warpNameShipBattle
    DEFINEWARP  -9495,   -127, -19015,  0, warpNameAnimTest
    DEFINEWARP  13440,   -127,  -5120,  0, warpNameWGshrine
    DEFINEWARP  -7040,   -127,  -8320,  0, warpNameNikTest
    DEFINEWARP   7680,   -127,  14080,  0, warpNameRollDemo
    DEFINEWARP -12800,   -127,  27520,  0, warpNameKamerian
    DEFINEWARP -10240,   -127,  27520,  0, warpNameDuster
    DEFINEWARP  -7040,   -127,  29440,  0, warpNameDiamondBay
    DEFINEWARP   -640,   -127,  28800,  0, warpNameDiscovery
    DEFINEWARP   3200,   -127,  28160,  0, warpNameWillow
    DEFINEWARP  12800,   -127,  30080,  0, warpNameOldKP
    DEFINEWARP -19200,   -127,  33920,  0, warpNameCloudRace2
