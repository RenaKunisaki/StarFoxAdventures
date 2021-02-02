.ascii "savemenu" # 8 byte file ID for debug
.set SAVEGAME_MENU_XPOS,    5
.set SAVEGAME_MENU_YPOS,   40
.set SAVEGAME_MENU_WIDTH, 310
.set SAVEGAME_MENU_HEIGHT,400
.set SAVEGAME_INFO_XPOS,  320
.set SAVEGAME_INFO_YPOS,   40
.set SAVEGAME_INFO_WIDTH, 310
.set SAVEGAME_INFO_HEIGHT,400
.set MAX_SAVED_OBJECTS,    63
.set SAVEGAME_MENU_NOPTS,   6

savegameMenu:
    # subroutine: runs the SaveGame menu.
    # expects r14 = mainLoop.
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    bl      menuHideHud
    bl      menuSetFixedWidth
    bl      savegameMenu_doInput
    bl      savegameMenu_Main
    b       menuEndSub

savegameMenu_itemStrs:
    .int s_savegameItem_Game    - mainLoop
    .int s_savegameItem_Krystal - mainLoop
    .int s_savegameItem_Fox     - mainLoop
    .int s_savegameItem_K2      - mainLoop
    .int s_savegameItem_F2      - mainLoop
    .int s_savegameItem_Objs    - mainLoop
    .int 0

savegameMenu_itemFuncs:
    .int savegameMenu_drawGameState     - mainLoop
    .int savegameMenu_drawKrystalState  - mainLoop
    .int savegameMenu_drawFoxState      - mainLoop
    .int savegameMenu_drawKrystalState2 - mainLoop
    .int savegameMenu_drawFoxState2     - mainLoop
    .int savegameMenu_drawSavedObjs     - mainLoop

savegameMenu_Main: # draw savegame info
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    # draw the boxes
    li      r3,  SAVEGAME_MENU_XPOS   # X
    li      r4,  SAVEGAME_MENU_YPOS   # Y
    li      r5,  SAVEGAME_MENU_WIDTH  # width
    li      r6,  SAVEGAME_MENU_HEIGHT # height
    bl      menuDrawBoxOpaque

    li      r3,  SAVEGAME_INFO_XPOS   # X
    li      r4,  SAVEGAME_INFO_YPOS   # Y
    li      r5,  SAVEGAME_INFO_WIDTH  # width
    li      r6,  SAVEGAME_INFO_HEIGHT # height
    bl      menuDrawBoxOpaque

    LOAD    r3,  0xFFFFFFFF
    bl      menuSetTextColor

    # get savegame
    LOADW   r15, pCurSaveGame
    cmpwi   r15, 0
    beq     .savegameMenu_noSave

    # r14: mainLoop
    # r15: saveGame
    # r17: selected line
    # r18: current line
    # r19: text X
    # r20: text Y
    # r21: lines

    # draw menu
    lbz     r17, (saveMenuIdx - mainLoop)(r14) # selected line
    li      r18, 0 # line idx
    li      r19, SAVEGAME_MENU_XPOS + 8 # line X pos
    li      r20, SAVEGAME_MENU_YPOS + 8 # line Y pos
    addi    r21, r14, savegameMenu_itemStrs - mainLoop # lines
.savegameMenu_drawNextItem:
    LOAD    r3,  0xFFFFFFFF
    cmpw    r18, r17 # cur line selected?
    bne     .savegameMenu_drawNotSel
    LOAD    r3,  0x00FFFFFF
.savegameMenu_drawNotSel:
    bl      menuSetTextColor
    slwi    r4,  r18, 2
    lwzx    r3,  r21, r4
    cmpwi   r3,  0
    beq     .savegameMenu_doneDraw
    add     r3,  r3,  r14
    bl      menuDrawStr2
    addi    r18, r18, 1
    addi    r20, r20, LINE_HEIGHT
    b       .savegameMenu_drawNextItem

.savegameMenu_doneDraw:
    # draw the selected info
    lbz     r17, (saveMenuIdx - mainLoop)(r14) # selected line
    slwi    r3,  r17, 2
    addi    r17, r14, savegameMenu_itemFuncs - mainLoop
    lwzx    r3,  r17, r3
    add     r3,  r3,  r14
    mtspr   CTR, r3
    bctr


savegameMenu_doInput:
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)
    lbz     r17, (saveMenuIdx - mainLoop)(r14)

    # check the buttons
    bl      menuGetInput
    # r3=buttons, r4=stick X, r5=stick Y,
    # r6=CX, r7=CY, r8=L, r9=R
    andi.   r10, r3,  PAD_BUTTON_B
    bne     .savegameMenu_close

    # check C stick
    cmpwi   r7,  -0x10
    blt     .savegameMenu_objListDown
    cmpwi   r7,   0x10
    bgt     .savegameMenu_objListUp

    # check analog stick
    cmpwi   r5,  -0x10
    blt     .savegameMenu_down
    cmpwi   r5,   0x10
    ble     menuEndSub

.savegameMenu_up: # up pressed
    subi    r17, r17, 1
.savegameMenu_up2:
    cmpwi   r17, 0
    bge     .savegameMenu_storeIdx
    addi    r17, r17, SAVEGAME_MENU_NOPTS - 1

.savegameMenu_down: # down pressed
    addi    r17, r17, 1
.savegameMenu_down2:
    cmpwi   r17, SAVEGAME_MENU_NOPTS
    blt     .savegameMenu_storeIdx
    li      r17, 0

.savegameMenu_storeIdx:
    stb     r17, (saveMenuIdx - mainLoop)(r14)
    li      r3,  MOVE_DELAY
.savegameMenu_storeIdx2:
    stb     r3,  (menuJustMoved - mainLoop)(r14)
    b       menuEndSub

.savegameMenu_close:
    bl      returnToMainMenu
    b       menuEndSub

.savegameMenu_noSave:
    addi    r3,  r14, s_savegameEmpty - mainLoop
    li      r5,  SAVEGAME_MENU_XPOS + 8 # X pos
    li      r6,  SAVEGAME_MENU_YPOS + 8 # Y pos
    bl      menuDrawStr
    b       menuEndSub

.savegameMenu_objListDown:
    lbz     r3,  (saveMenuObjIdx - mainLoop)(r14)
    addi    r3,  r3,  1
    cmpwi   r3,  MAX_SAVED_OBJECTS
    blt     .savegameMenu_objListStore
    li      r3,  0
.savegameMenu_objListStore:
    stb     r3,  (saveMenuObjIdx - mainLoop)(r14)
    li      r3,  3
    b       .savegameMenu_storeIdx2

.savegameMenu_objListUp:
    lbz     r3,  (saveMenuObjIdx - mainLoop)(r14)
    cmpwi   r3,  0
    beq     .savegameMenu_objListTop
    subi    r3,  r3,  1
    b       .savegameMenu_objListStore
.savegameMenu_objListTop:
    li      r3,  MAX_SAVED_OBJECTS - 1
    b       .savegameMenu_objListStore


savegameMenu_drawGameState:
    # draw current character
    addi    r4,  r14, s_savegameCurChr - mainLoop
    addi    r5,  r14, s_Krystal - mainLoop
    lbz     r6,  0x20(r15)
    cmpwi   r6,  0
    beq     .savegameMenu_isKrystal
    addi    r5,  r14, s_Fox - mainLoop

.savegameMenu_isKrystal:
    addi    r6,  r15, 0x1C # save file name
    li      r19, SAVEGAME_INFO_XPOS + 8 # X pos
    li      r20, SAVEGAME_INFO_YPOS + 8 # Y pos
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    # draw flags
    addi    r4,  r14, s_savegameUnkFlags - mainLoop
    lbz     r5,  0x21(r15)
    lbz     r6,  0x22(r15)
    lbz     r7,  0x23(r15)
    lhz     r8,  0x1A(r15)
    bl      menuPrintf
    b       menuEndSub


savegameMenu_drawKrystalState2:
    li      r3,  0
savegameMenu_drawCharState2:
    LOADWH  r15, pRestartPoint
    LOADWL2 r15, pRestartPoint, r15
    b      savegameMenu_drawCharState

savegameMenu_drawFoxState2:
    li      r3,  1
    b      savegameMenu_drawCharState2

savegameMenu_drawKrystalState:
    li     r3,  0
    b      savegameMenu_drawCharState

savegameMenu_drawFoxState:
    li     r3,  1

savegameMenu_drawCharState:
    # draw character state
    # r3:  which character
    # r15: savegame*
    cmpwi   r15, 0
    beq     .savegameMenu_drawCharState_noSave
    mr      r22, r3
    mulli   r3,  r3,  0x0C # size of char state
    add     r21, r15, r3
    li      r19, SAVEGAME_INFO_XPOS + 8 # X pos
    li      r20, SAVEGAME_INFO_YPOS + 8 # string Y pos

    # HP, MP
    addi    r4,  r14, s_savegameHPMP - mainLoop
    lbz     r5,  0x00(r21) # cur HP
    lbz     r6,  0x01(r21) # max HP
    lhz     r7,  0x04(r21) # cur MP
    lhz     r8,  0x06(r21) # max MP
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    # lives, money
    addi    r4,  r14, s_savegameLivesMoney - mainLoop
    lbz     r5,  0x09(r21) # cur lives
    lbz     r6,  0x0A(r21) # max lives
    lbz     r7,  0x08(r21) # cur money
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    # unknown
    addi    r4,  r14, s_savegameUnkChrState - mainLoop
    lbz     r5,  0x02(r21)
    lbz     r6,  0x03(r21)
    lbz     r7,  0x0B(r21)
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    # coords
    mulli   r3,  r22, 0x10 # size of char pos
    add     r21, r15, r3

    addi    r4,  r14, s_savegameCoordX - mainLoop
    lfs     f1,  0x684(r21)
    MAGIC_FLOAT_INCANTATION
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    addi    r4,  r14, s_savegameCoordY - mainLoop
    lfs     f1,  0x688(r21)
    MAGIC_FLOAT_INCANTATION
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    addi    r4,  r14, s_savegameCoordZ - mainLoop
    lfs     f1,  0x68C(r21)
    MAGIC_FLOAT_INCANTATION
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    # grid, rot, unk
    addi    r4,  r14, s_savegameLayer - mainLoop
    lbz     r5,  0x690(r21) # rot
    lbz     r6,  0x691(r21) # grid
    lhz     r7,  0x692(r21) # unk
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    # map name
    lfs     f30, (f_mapCellScale - mainLoop)(r14)
    lfs     f1,  0x684(r21) # X
    lfs     f3,  0x68C(r21) # Z
    fdivs   f1,  f1, f30
    fctiwz  f1,  f1
    stfd    f1,  SP_FLOAT_TMP(r1)
    lwz     r3,  SP_FLOAT_TMP+4(r1) # X
    fdivs   f3,  f3, f30
    fctiwz  f3,  f3
    stfd    f3,  SP_FLOAT_TMP(r1)
    lwz     r4,  SP_FLOAT_TMP+4(r1) # Z
    lbz     r5,  0x691(r21) # grid
    CALL    mapCoordsToId
    addi    r7,  r14, fmt_warpListNoMap - mainLoop
    cmpwi   r3,  0
    blt     .saveMenu_noMap

    # get the map name
    slwi    r5,  r3,  2
    LOAD    r4,  mapNames
    lwzx    r7,  r4,  r5

    # draw map name
.saveMenu_noMap:
    addi    r4,  r14, s_savegameMap - mainLoop
    mr      r5,  r7
.saveMenu_printAndEnd:
    bl      menuPrintf
    b       menuEndSub

.savegameMenu_drawCharState_noSave:
    li      r19, SAVEGAME_INFO_XPOS + 8 # X pos
    li      r20, SAVEGAME_INFO_YPOS + 8 # string Y pos
    addi    r4,  r14, s_savegameEmpty - mainLoop
    b       .saveMenu_printAndEnd

savegameMenu_drawSavedObjs:
    li      r19, SAVEGAME_INFO_XPOS + 8 # X pos
    li      r20, SAVEGAME_INFO_YPOS + 8 # string Y pos
    addi    r21, r15, 0x0168
    lbz     r3,  (saveMenuObjIdx - mainLoop)(r14)
    mr      r22, r3
    slwi    r3,  r3,  4
    add     r21, r21, r3

.savegameMenu_drawNextObj:
    lwz     r5,  0x00(r21) # ID
    #cmpwi   r5,  0
    #beq     menuEndSub

    lfs     f1,  0x04(r21) # X
    fctiwz  f1,  f1
    stfd    f1,  SP_FLOAT_TMP(r1)
    lwz     r6,  SP_FLOAT_TMP+4(r1)

    lfs     f1,  0x08(r21) # Y
    fctiwz  f1,  f1
    stfd    f1,  SP_FLOAT_TMP(r1)
    lwz     r7,  SP_FLOAT_TMP+4(r1)

    lfs     f1,  0x0C(r21) # Z
    fctiwz  f1,  f1
    stfd    f1,  SP_FLOAT_TMP(r1)
    lwz     r8,  SP_FLOAT_TMP+4(r1)

    addi    r4,  r14, s_savegameSavedObj - mainLoop
    bl      menuPrintf
    cmpwi   r20, SAVEGAME_INFO_YPOS + SAVEGAME_INFO_HEIGHT - LINE_HEIGHT
    bge     menuEndSub

    addi    r20, r20, LINE_HEIGHT
    addi    r21, r21, 0x10
    addi    r22, r22, 1
    cmpwi   r22, MAX_SAVED_OBJECTS
    bge     menuEndSub
    b       .savegameMenu_drawNextObj
