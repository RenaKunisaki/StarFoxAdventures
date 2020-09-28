.set SPAWN_MENU_XPOS,    15
.set SPAWN_MENU_YPOS,    50
.set SPAWN_MENU_WIDTH,  620
.set SPAWN_MENU_HEIGHT, 360
.set SPAWN_MENU_EXTRA_LINES, 3
.set SPAWN_MENU_CURSOR_X_OFFS, 148

spawnMenu:
    # subroutine: runs the Spawn Object menu.
    # expects r14 = mainLoop.
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    bl      menuHideHud
    bl      menuSetFixedWidth
    bl      spawnMenu_doInput
    bl      spawnMenu_Main
    b       menuEndSub

spawnMenu_Main: # draw the menu
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    # draw the boxes
    li      r3,  SPAWN_MENU_XPOS   # X
    li      r4,  SPAWN_MENU_YPOS   # Y
    li      r5,  SPAWN_MENU_WIDTH  # width
    li      r6,  SPAWN_MENU_HEIGHT # height
    bl      menuDrawBoxOpaque

    # draw instructions
    LOAD    r3,  0xFFFFFFFF
    bl      menuSetTextColor
    addi    r3,  r14, s_spawnInstrs - mainLoop
    li      r5,  SPAWN_MENU_XPOS + 8
    li      r6,  SPAWN_MENU_YPOS + SPAWN_MENU_HEIGHT - 16
    bl      menuDrawStr

    # draw cursor
    lbz     r3,  (spawnMenuCursorX - mainLoop)(r14)
    lbz     r4,  (spawnMenuCursor  - mainLoop)(r14)
    cmpwi   r4,  1
    beq     .spawnMenu_skipCursor # don't draw for num params
    mulli   r3,  r3,  11
    addi    r3,  r3,  SPAWN_MENU_CURSOR_X_OFFS
    mulli   r4,  r4,  LINE_HEIGHT
    addi    r4,  r4,  SPAWN_MENU_YPOS + 4
    li      r5,  16 # width
    li      r6,  16 # height
    bl      menuDrawBoxOpaque
.spawnMenu_skipCursor:

    # set up the menu
    li      r19, SPAWN_MENU_XPOS + 8 # line X pos
    li      r20, SPAWN_MENU_YPOS + 8 # line Y pos
    lbz     r21, (spawnMenuNumParams - mainLoop)(r14)
    addi    r21, r21, SPAWN_MENU_EXTRA_LINES
    lbz     r22, (spawnMenuCursor - mainLoop)(r14)
    li      r23, 0 # current line

    # draw the menu
.spawnMenu_drawNextItem:
    cmpw    r22, r23
    beq     .spawnMenu_drawSelected
    LOAD    r3,  0xFFFFFFFF
    b       .spawnMenu_setColor
.spawnMenu_drawSelected:
    LOAD    r3,  0x00FFFFFF

.spawnMenu_setColor:
    bl      menuSetTextColor
    cmpwi   r23, 0
    beq     .spawnMenu_drawType
    cmpwi   r23, 1
    beq     .spawnMenu_drawNumParams
    cmpwi   r23, 2
    beq     .spawnMenu_drawFlags

    # else, draw params
    addi    r4,  r14, s_spawnParam - mainLoop
    subi    r5,  r23, 3
    slwi    r5,  r5,  2
    addi    r6,  r5,  spawnMenuParams - mainLoop
    lwzx    r6,  r6,  r14
    b       .spawnMenu_drawLine


.spawnMenu_drawType: # draw object type
    addi    r4,  r1,  SP_FLOAT_TMP # reused as second str buf
    lhz     r3,  (spawnMenuType - mainLoop)(r14)
    bl      spawnMenu_getObjName
    addi    r4,  r14, s_spawnType - mainLoop
    lhz     r5,  (spawnMenuType - mainLoop)(r14) # show def ID
    mr      r6,  r3   # show real ID
    addi    r7,  r1,  SP_FLOAT_TMP # show name
    b       .spawnMenu_drawLine

.spawnMenu_drawNumParams: # draw number of params
    addi    r4,  r14, s_spawnNumParams - mainLoop
    lbz     r5,  (spawnMenuNumParams - mainLoop)(r14)
    b       .spawnMenu_drawLine

.spawnMenu_drawFlags: # draw spawn flags
    addi    r4,  r1,  SP_FLOAT_TMP # reused as second str buf
    lbz     r3,  (spawnMenuFlags - mainLoop)(r14)
    bl      spawnMenu_printBinary
    addi    r4,  r14, s_spawnFlags - mainLoop
    addi    r5,  r1,  SP_FLOAT_TMP
    lbz     r6,  (spawnMenuFlags - mainLoop)(r14) # also show hex
    #b       .spawnMenu_drawLine

.spawnMenu_drawLine:
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT
    addi    r23, r23, 1 # next line
    cmpw    r23, r21
    bne     .spawnMenu_drawNextItem

    b       menuEndSub


spawnMenu_printBinary: # print 8 binary digits of value r3 to buf at r4
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r14,  r4
    li      r15,  0
    stb     r15,  8(r14) # null terminator
    subi    r14,  r14,  1 # for stbu
    li      r15,  7
.printBinary_next:
    srw     r16,  r3,   r15
    andi.   r16,  r16,  1
    addi    r16,  r16,  '0'
    stbu    r16,  1(r14)
    cmpwi   r15,  0
    beq     menuEndSub
    subi    r15,  r15,  1
    b       .printBinary_next


spawnMenu_getObjName: # copy object name to buf at r4
    # r3: objDef ID
    # returns r3 = real ID
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    # store null terminator
    li      r15, 0
    stb     r15, 11(r4)
    cmpwi   r3,  MAX_OBJIND_ID
    bge     .spawnMenu_getObjName_invalid

    # get s16 at OBJINDEX.BIN[r3*2]
    LOAD    r15, dataFileBuffers
    lwz     r16, (OBJINDEX_BIN*4)(r15) # r16 = OBJINDEX.BIN
    slwi    r17, r3,  1
    lhax    r17, r17, r16 # get index
    cmpwi   r17, 0
    blt     .spawnMenu_getObjName_noRemap # if that's -1, just use r3
    # else, use that value
    mr      r3,  r17
.spawnMenu_getObjName_noRemap:

    # look that up in OBJECTS.TAB
    lwz     r16, (OBJECTS_TAB*4)(r15)
    slwi    r17, r3,  2
    lwzx    r17, r17, r16 # get offset

    # look at the corresponding offset + 0x91 in OBJECTS.BIN
    addi    r17, r17, 0x91
    lwz     r16, (OBJECTS_BIN*4)(r15)
    add     r16, r16, r17

    # copy 11 chars to output
    li      r15, 0
.spawnMenu_getObjName_nextChar:
    lbzx    r17, r16, r15
    stbx    r17, r4,  r15
    addi    r15, r15, 1
    cmpwi   r15, 11
    blt     .spawnMenu_getObjName_nextChar

    b       menuEndSub

.spawnMenu_getObjName_invalid:
    lis      r15, ('N' << 8) | '/'
    ori      r15, r15, ('A' << 8)
    stw      r15, 0(r4) # store "N/A"
    li       r3,  -1
    b       menuEndSub


spawnMenu_doInput:
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    # check the buttons
    bl      menuGetInput
    lbz     r15, (spawnMenuCursor - mainLoop)(r14)
    # r3=buttons, r4=stick X, r5=stick Y,
    # r6=CX, r7=CY, r8=L, r9=R
    andi.   r10, r3,  PAD_BUTTON_B
    bne     .spawnMenu_close
    andi.   r10, r3,  PAD_BUTTON_MENU
    bne     .spawnMenu_doSpawn_normal
    andi.   r10, r3,  PAD_BUTTON_Z
    bne     .spawnMenu_doSpawn_neg
    andi.   r10, r3,  PAD_BUTTON_X
    bne     .spawnMenu_increment
    andi.   r10, r3,  PAD_BUTTON_Y
    bne     .spawnMenu_decrement

    # check analog stick
    cmpwi   r5,   0x10
    bgt     .spawnMenu_up
    cmpwi   r5,  -0x10
    blt     .spawnMenu_down
    cmpwi   r4,   0x10
    bgt     .spawnMenu_right
    cmpwi   r4,  -0x10
    blt     .spawnMenu_left


    b       menuEndSub

.spawnMenu_close:
    bl      returnToMainMenu
    b       menuEndSub

.spawnMenu_up: # up pressed
    subi    r15, r15, 1
    b       .spawnMenu_moveCursor

.spawnMenu_down: # down pressed
    addi    r15, r15, 1

.spawnMenu_moveCursor: # range check
    lbz     r3,  (spawnMenuNumParams - mainLoop)(r14)
    addi    r3,  r3, SPAWN_MENU_EXTRA_LINES
    cmpwi   r15, 0
    blt     .spawnMenu_cursorWrapTop
    cmpw    r15, r3
    blt     .spawnMenu_storeCursor
    li      r15, 0

.spawnMenu_storeCursor:
    stb     r15, (spawnMenuCursor - mainLoop)(r14)
.spawnMenu_afterMove:
    li      r3,  15
    stb     r3,  (menuJustMoved - mainLoop)(r14)
    b       menuEndSub

.spawnMenu_cursorWrapTop:
    subi    r15, r3,  1
    b       .spawnMenu_storeCursor

.spawnMenu_left: # left pressed
    li      r3,  -1
    b       .spawnMenu_adjustX

.spawnMenu_right: # right pressed
    li      r3,  1

.spawnMenu_adjustX:
    lbz     r4,  (spawnMenuCursorX - mainLoop)(r14)
    add     r4,  r4,  r3
    andi.   r4,  r4, 7
    stb     r4,  (spawnMenuCursorX - mainLoop)(r14)
    b       .spawnMenu_afterMove

.spawnMenu_decrement: # ? pressed - subtract 1 from selected
    li      r3,  -1
    b       .spawnMenu_adjust

.spawnMenu_increment: # ? pressed - add 1 to selected
    li      r3,  1

.spawnMenu_adjust:
    lbz     r16, (spawnMenuCursorX - mainLoop)(r14)
    cmpwi   r15, 0
    beq     .spawnMenu_adjType
    cmpwi   r15, 1
    beq     .spawnMenu_adjNumParams
    cmpwi   r15, 2
    beq     .spawnMenu_adjFlags

    # else adjust parameter
    li      r6,  7
    sub     r16, r6,  r16 # invert cursor direction
    subi    r15, r15, 3
    slwi    r15, r15, 2
    addi    r17, r15, spawnMenuParams - mainLoop
    lwzx    r4,  r17, r14
    bl      spawnMenu_adjustDigit
    stwx    r4,  r17, r14
    b       .spawnMenu_afterMove

.spawnMenu_adjType:
    cmpwi   r16, 3
    ble     .spawnMenu_adjType_notEnd
    li      r16, 3
.spawnMenu_adjType_notEnd:
    li      r5,  3
    sub     r16, r5,  r16 # invert cursor direction
    lhz     r4,  (spawnMenuType - mainLoop)(r14)
    bl      spawnMenu_adjustDigit
    sth     r4,  (spawnMenuType - mainLoop)(r14)
    b       .spawnMenu_afterMove

.spawnMenu_adjNumParams:
    lbz     r4,  (spawnMenuNumParams - mainLoop)(r14)
    add     r4,  r4,  r3
    cmpwi   r4,  0
    blt     .spawnMenu_paramsWrapTop
    cmpwi   r4,  SPAWN_MENU_MAX_PARAMS
    blt     .spawnMenu_storeNumParams
    li      r4,  0
.spawnMenu_storeNumParams:
    stb     r4,  (spawnMenuNumParams - mainLoop)(r14)
    b       .spawnMenu_afterMove
.spawnMenu_paramsWrapTop:
    li      r4,  SPAWN_MENU_MAX_PARAMS
    b       .spawnMenu_storeNumParams

.spawnMenu_adjFlags:
    li      r3,  7
    sub     r16, r3,  r16 # invert cursor direction
    lbz     r4,  (spawnMenuFlags - mainLoop)(r14)
    li      r3,  1
    slw     r3,  r3,  r16
    xor     r4,  r4,  r3
    stb     r4,  (spawnMenuFlags - mainLoop)(r14)
    b       .spawnMenu_afterMove

spawnMenu_adjustDigit: # adjust one digit at a time in a hex number.
    # r3: amount to adjust by.
    # r4: value to adjust.
    # r16: digit to adjust, with 0 being rightmost.
    # clobbers: r3, r5, r6, r7, r8, r9
    # returns adjusted value in r4
    mflr    r9
    slwi    r16, r16, 2 # adjust hex digit
    # make a mask so that we can adjust one digit at a time
    # without affecting the others.
    li      r5,  0xF
    slw     r5,  r5,  r16 # mask
    slw     r3,  r3,  r16 # adjust correct digit

    li      r7,  -1
    xor     r8,  r5,  r7 # invert mask
    and     r8,  r8,  r4 # get only unchanged part
    add     r4,  r4,  r3 # do change
    and     r6,  r4,  r5 # get only changed part
    or      r4,  r8,  r6
    mtlr    r9
    blr

.spawnMenu_doSpawn_normal:
    lhz     r4,  (spawnMenuType - mainLoop)(r14)
    b       .spawnMenu_doSpawn

.spawnMenu_doSpawn_neg:
    lhz     r4,  (spawnMenuType - mainLoop)(r14)
    neg     r4,  r4

.spawnMenu_doSpawn: # actually spawn the object.
    lbz     r3,  (spawnMenuNumParams - mainLoop)(r14)
    slwi    r3,  r3,  2
    addi    r3,  r3,  0x18
    CALL    objAlloc

    LOADW   r21, pCamera
    cmpwi   r21, 0
    beq     .spawnMenu_noCamera
    lwz     r16, 0xA4(r21)
    cmpwi   r16, 0
    beq     .spawnMenu_noCamera # no focus object

    # spawn at location of focused object.
    lwz     r4,  0x0C(r16)
    stw     r4,  0x08(r3)
    lfs     f1,  0x10(r16)
    lfs     f2,  (.spawnMenu_yOffs - mainLoop)(r14)
    fadds   f1,  f1,  f2 # offset to near player's eye level
    stfs    f1,  0x0C(r3) # so it won't fall through ground
    lwz     r4,  0x14(r16)
    stw     r4,  0x10(r3)

.spawnMenu_noCamera:
    lbz     r4,  (spawnMenuFlags - mainLoop)(r14)
    li      r5, -1 # map
    li      r6, -1 # objNo
    li      r7,  0 # matrix*
    CALL    objInstantiateCharacter
    #bl      returnToMainMenu
    b       menuEndSub

#.spawnMenu_yOffs: .float 40
.spawnMenu_yOffs: .float 0
