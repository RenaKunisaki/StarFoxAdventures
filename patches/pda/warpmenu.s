.set WARP_MENU_XPOS,   20
.set WARP_MENU_YPOS,   56
.set WARP_MENU_WIDTH, 600
.set WARP_MENU_HEIGHT,300
.set MAX_WARP,0x80

warpMenu:
    # subroutine: runs the Warp menu.
    # expects r14 = mainLoop.
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r0
    stw   r0, SP_LR_SAVE(r1)
    stmw  r13, SP_GPR_SAVE(r1)

    #bl      menuHideHud
    bl      menuSetFixedWidth
    bl      warpMenu_doInput
    bl      warpMenu_Main
    b       menuEndSub


warpMenu_Main: # draw list of warps.
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r0
    stw   r0, SP_LR_SAVE(r1)
    stmw  r13, SP_GPR_SAVE(r1)

    # get the warp table
    LOADW r15, dataFileBuffers + (WARPTAB_BIN * 4)
    cmpwi r15, 0
    beq   menuEndSub

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
    lfs     f30, (f_mapCellScale - mainLoop)(r14)

    # mapCoordsToId looks relative to current layer,
    # so temporarily reset that to 0
    LOADWH  r9,  curMapLayer
    LOADBL2 r31, curMapLayer, r9
    li      r5,  0
    STOREB  r5,  curMapLayer, r9

.warpMenu_nextWarp:
    slwi    r9,  r17, 4
    add     r9,  r9, r15 # r4 = warp entry*
    lfs     f1,  0x00(r9) # X
    #lfs     f2,  0x04(r9) # Y
    lfs     f3,  0x08(r9) # Z
    lha     r5,  0x0C(r9) # layer
    #lha     r7,  0x0E(r9) # angle

    fdivs   f1, f1, f30
    fctiwz  f1, f1
    stfd    f1, SP_FLOAT_TMP(r1)
    lwz     r3, SP_FLOAT_TMP+4(r1) # X
    fdivs   f3, f3, f30
    fctiwz  f3, f3
    stfd    f3, SP_FLOAT_TMP(r1)
    lwz     r4, SP_FLOAT_TMP+4(r1) # Z
    CALL    mapCoordsToId
    addi    r7,  r14, fmt_warpListNoMap - mainLoop
    cmpwi   r3, 0
    blt     .warpMenu_noMap

    # get the map name
    slwi    r5, r3, 2
    LOAD    r4, mapNames
    lwzx    r7, r4, r5

.warpMenu_noMap:
    # make line
    andi.   r6,  r3, 0xFF # map ID
    addi    r3,  r1, SP_STR_BUF
    addi    r4,  r14, fmt_warpListEntry - mainLoop
    mr      r5,  r17 # warp idx
    CALL    sprintf

    addi    r3,  r1, SP_STR_BUF
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  WARP_MENU_XPOS + 8  # X pos
    mr      r6,  r20 # Y pos
    CALL    gameTextShowStr

    LOAD    r3, 0xFFFFFFFF
    bl      menuSetTextColor

    # next line
    addi    r17, r17, 1
    cmpwi   r17, MAX_WARP # last entry?
    bge     .warpMenu_drawSelected
    addi    r20, r20, LINE_HEIGHT
    cmpwi   r20, WARP_MENU_YPOS + WARP_MENU_HEIGHT - LINE_HEIGHT
    blt     .warpMenu_nextWarp

.warpMenu_drawSelected:
    # draw selected item
    #LOAD    r3, 0xFFFFFFFF
    #bl      menuSetTextColor

    lbz     r17, (warpMenuIdx - mainLoop)(r14)
    slwi    r9,  r17, 4
    add     r9,  r9, r15 # r4 = warp entry*
    lfs     f1,  0x00(r9) # X
    lfs     f2,  0x04(r9) # Y
    lfs     f3,  0x08(r9) # Z
    lha     r5,  0x0C(r9) # layer
    lha     r6,  0x0E(r9) # angle
    addi    r3,  r1, SP_STR_BUF
    addi    r4,  r14, fmt_warpListCoords - mainLoop
    # magic required to make floats print correctly
    # no idea what this does
    creqv 4*cr1+eq,4*cr1+eq,4*cr1+eq
    CALL    sprintf

    addi    r3,  r1, SP_STR_BUF
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  WARP_MENU_XPOS + 8  # X pos
    li      r6,  WARP_MENU_YPOS + WARP_MENU_HEIGHT + 8 # Y pos
    CALL    gameTextShowStr

    # instructions
    #addi    r3, r14, fmt_warpListInstrs - mainLoop
    #li      r4,  MENU_TEXTBOX_ID # box type
    #li      r5,  WARP_MENU_XPOS + 8  # X pos
    #li      r6,  WARP_MENU_YPOS + WARP_MENU_HEIGHT + 24 # Y pos
    #CALL    gameTextShowStr

    LOADWH  r9,  curMapLayer
    STOREB  r31, curMapLayer, r9

    b       menuEndSub

warpMenu_doInput:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0, SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)
    lbz     r17, (warpMenuIdx - mainLoop)(r14)

    # check the buttons
    bl      menuGetInput
    # r3=buttons, r4=stick X, r5=stick Y,
    # r6=CX, r7=CY, r8=L, r9=R
    andi.   r10, r3, PAD_BUTTON_B
    bne     .warpMenu_close
    andi.   r10, r3, PAD_BUTTON_A
    bne     .warpMenu_doWarp
    andi.   r10, r3, PAD_BUTTON_Z
    bne     .warpMenu_doSwap

    # check analog stick
    cmpwi   r5, 0x10
    bgt     .warpMenu_up
    cmpwi   r5, -0x10
    blt     .warpMenu_down

    # check C stick - same as analog but no delay
    cmpwi   r7, 0x10
    bgt     .warpMenu_up
    cmpwi   r7, -0x10
    blt     .warpMenu_down

    # check L/R - jump by page
    cmpwi   r8, 0x04
    bgt     .warpMenu_prevPage
    cmpwi   r9, 0x04
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
    andi.   r7, r7, 0xF0 # deadzone
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
    LOADW   r15, dataFileBuffers + (WARPTAB_BIN * 4)
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
    lbz     r4,  0x0E(r9) # rotX
    stb     r4,  0x0C(r3)
    lbz     r4,  0x0D(r9) # layer
    stb     r4,  0x0D(r3)
    CALL    0x800e9be0    # loadMapForCurrentSaveGame
    b       .warpMenu_close

.warpMenu_doSwap:
    LOAD    r18, 0x803a32a8 # saveData.curSaveGame
    lbz     r3,  0x20(r18) # character
    xori    r3,  r3,  1
    stb     r3,  0x20(r18)
    b       .warpMenu_doWarp
