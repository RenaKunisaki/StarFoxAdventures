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

    #bl    menuHideHud
    bl     warpMenu_doInput
    bl     warpMenu_Main
    b      menuEndSub


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
    addi    r3, r14, fmt_warpListInstrs - mainLoop
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  WARP_MENU_XPOS + 8  # X pos
    li      r6,  WARP_MENU_YPOS + WARP_MENU_HEIGHT + 24 # Y pos
    CALL    gameTextShowStr

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
    bne     .warpMenu_doJump

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
    CALL    0x80056f7c # unloadMap
    #lbz     r3, (warpMenuIdx - mainLoop)(r14)

    lbz     r17, (warpMenuIdx - mainLoop)(r14)
    slwi    r9,  r17, 4
    add     r9,  r9, r15 # r4 = warp entry*
    lfs     f1,  0x00(r9) # X
    lfs     f2,  0x04(r9) # Y
    lfs     f3,  0x08(r9) # Z
    lha     r3,  0x0C(r9) # layer
    #CALL    0x80020770
    # this actually prevents the warp...
    # I suspect the reason maps aren't initialized properly when we warp
    # into them is just that the game state isn't right. things are set
    # up to appear at certain points in the game and we're not at that
    # point, so they don't show up.
    # but that doesn't explain why they do show up if we wander into
    # a cave.
    # it might just be loading based on the player's cell offset?

    #CALL     0x800481b0 # map ID -> dir Idx

    #LOADWH  r8, 0x803dcaf8
    #LOADWL2 r3, 0x803dcaf8, r8
    #CALL    0x80042f78 # loadMapAndParent

    #li      r3, 0x1E
    #li      r4, 1
    #CALL    0x800d7b04 # screen transition

    lbz     r3, (warpMenuIdx - mainLoop)(r14)
    li      r4, 0
.warpMenu_doCallWarp:
    #CALL    warpToMap

    #li      r3, 7
    #li      r4, 0
    #CALL    0x80043560 # lockLevel

    #li      r3, 0
    #li      r4, 0
    #li      r5, 1
    #CALL    0x8004350C # unlockLevel

    # update player cell
    lbz     r17, (warpMenuIdx - mainLoop)(r14)
    slwi    r9,  r17, 4
    add     r9,  r9, r15 # r4 = warp entry*
    lfs     f1,  0x00(r9) # X
    lfs     f2,  0x04(r9) # Y
    lfs     f3,  0x08(r9) # Z
    lfs     f4,  (f_mapCellScale - mainLoop)(r14)
    LOADWH  r4,  0x803dcdd8 # playerMapOffsetX
    STOREF  f1,  0x803dcdd8, r4
    STOREF  f3,  0x803dcddc, r4
    LOADW   r5,  0x803dcea8
    stfs    f1,  0x0C(r5)
    stfs    f2,  0x10(r5)
    stfs    f3,  0x14(r5)

    LOADW   r16, pPlayer
    stfs    f1,  0x0C(r16)
    stfs    f2,  0x10(r16)
    stfs    f3,  0x14(r16)

    fctiwz  f0,  f1
    stfd    f0,  SP_FLOAT_TMP(r1)
    lwz     r3,  (SP_FLOAT_TMP+4)(r1)
    STOREW  r3,  0x803dcdc8, r4
    fctiwz  f0,  f3
    stfd    f0,  SP_FLOAT_TMP(r1)
    lwz     r3,  (SP_FLOAT_TMP+4)(r1)
    STOREW  r3,  0x803dcdcc, r4

    #fdivs   f1,  f1,  f4
    #fdivs   f3,  f3,  f4
    #fctiwz  f0,  f1
    #stfd    f0,  SP_FLOAT_TMP(r1)
    #lwz     r3,  (SP_FLOAT_TMP+4)(r1)
    #STOREW  r3,  0x803dcdd0, r4
    #fctiwz  f0,  f3
    #stfd    f0,  SP_FLOAT_TMP(r1)
    #lwz     r3,  (SP_FLOAT_TMP+4)(r1)
    #STOREW  r3,  0x803dcdd4, r4

    #li      r3, 7
    #STOREW  r3, 0x803dcec8, r4 # curMapId

    LOADWL2 r3, 0x803dcde8, r4
    ori     r3, r3, 0x5000

    #CALL    waitNextFrame

    lbz     r3, (warpMenuIdx - mainLoop)(r14)
    li      r4, 0
    CALL    warpToMap



    #li      r3, 1
    #LOADWH  r4, 0x803dca41
    #STOREB  r3, 0x803dca41, r4 # shouldLoadMap
    #li      r3, 0x7
    #STOREW  r3, 0x803dcaf8, r4 # mapNoToLoad
    ##STOREB  r3, 0x803dca39, r4 # anyQueuedLoads - prevents warp
    #STOREW  r3, 0x803dcaf4, r4
    ##CALL    0x800573f0 # mapReloadWithFadeout
    b       .warpMenu_close

.warpMenu_doJump:
    lbz     r3, (warpMenuIdx - mainLoop)(r14)
    li      r4, 1 # this param might mean "don't unload current map"?
    b       .warpMenu_doCallWarp

    # this doesn't really work
    #lbz     r17, (warpMenuIdx - mainLoop)(r14)
    #slwi    r9,  r17, 4
    #add     r9,  r9, r15 # r4 = warp entry*
    #lfs     f1,  0x00(r9) # X
    #lfs     f2,  0x04(r9) # Y
    #lfs     f3,  0x08(r9) # Z
    #LOADW   r16, pPlayer
    #stfs    f1,  0x0C(r16)
    #stfs    f2,  0x10(r16)
    #stfs    f3,  0x14(r16)
    #CALL    0x80020748 # mapReload
    b       .warpMenu_close

#f_mapCellScale: .float 640
