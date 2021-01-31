.ascii "bitmenu " # 8 byte file ID for debug

.set GAMEBIT_MENU_XPOS,   30
.set GAMEBIT_MENU_YPOS,  100
.set GAMEBIT_MENU_WIDTH, 560
.set GAMEBIT_MENU_HEIGHT,328
.set MAX_GAMEBIT,0x0F58

gamebitMenu:
    # subroutine: runs the GameBits menu.
    # expects r14 = mainLoop.
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    #bl     menuHideHud
    bl      menuSetFixedWidth
    bl      gamebitMenu_doInput
    bl      gamebitMenu_Main
    b       menuEndSub


gamebitMenu_Main: # draw list of bits.
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    # get the bit table
    LOADW   r15, 0x803dcadc
    cmpwi   r15, 0
    beq     menuEndSub

    # draw the box
    li      r3,  GAMEBIT_MENU_XPOS   # X
    li      r4,  GAMEBIT_MENU_YPOS   # Y
    li      r5,  GAMEBIT_MENU_WIDTH  # width
    li      r6,  GAMEBIT_MENU_HEIGHT # height
    li      r20, 255 # opacity
    bl      menuDrawBox

    # first item (selected) in blue
    LOAD    r3,  0x00FFFFFF
    bl      menuSetTextColor

    # draw the header
    addi    r3,  r14, fmt_bitListHeader - mainLoop
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  GAMEBIT_MENU_XPOS + 8 # X pos
    li      r6,  GAMEBIT_MENU_YPOS + 8 # Y pos
    CALL    gameTextShowStr

    # draw instructions
    addi    r3,  r14, fmt_bitListInstrs - mainLoop
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  GAMEBIT_MENU_XPOS + 8 # X pos
    li      r6,  GAMEBIT_MENU_YPOS + 8 + (GAMEBIT_MENU_HEIGHT-22) # Y pos
    CALL    gameTextShowStr

    lhz     r17, (bitMenuIdx - mainLoop)(r14)
    li      r20, GAMEBIT_MENU_YPOS + LINE_HEIGHT + 8 # string Y pos

.gameBitMenu_nextBit:
    slwi    r4,  r17, 2
    lwzx    r18, r4, r15   # r18 = bit def

    # get value
    mr      r3,  r17
    CALL    mainGetBit
    mr      r21, r3

    # get text
    # of course the actual description is only loaded on the title screen,
    # other maps only have the hint, making this useless.
    addi    r3,  r14, emptyStr - mainLoop
    andi.   r0,  r18, 0x2000 # has hint text?
    beq     .gameBitMenu_noText
    andi.   r3,  r18, 0xFF
    addi    r3,  r3,  0xF4
    CALL    gameTextGet
    lwz     r3,  8(r3)
    lwz     r3,  0(r3)
    #addi    r3,  r3,  16

.gameBitMenu_noText:
    mr      r8,  r3
    # make line
    # r5:bit r6:tbl r7:val r8:text
    addi    r3,  r1, SP_STR_BUF
    addi    r4,  r14, fmt_bitListEntry - mainLoop
    mr      r5,  r17 # bit idx
    rlwinm  r6,  r18, 32-14, 0x0003 # table idx
    mr      r7,  r21
    CALL    sprintf

    addi    r3,  r1, SP_STR_BUF
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  GAMEBIT_MENU_XPOS + 8  # X pos
    mr      r6,  r20 # Y pos
    CALL    gameTextShowStr

    LOAD    r3,  0xFFFFFFFF
    bl      menuSetTextColor

    # next line
    addi    r17, r17, 1
    cmpwi   r17, MAX_GAMEBIT # last bit?
    bge     .gameBitMenu_drawCursor
    addi    r20, r20, LINE_HEIGHT
    cmpwi   r20, GAMEBIT_MENU_YPOS + GAMEBIT_MENU_HEIGHT - LINE_HEIGHT
    blt     .gameBitMenu_nextBit

.gameBitMenu_drawCursor:
    # draw box at cursor position
    lbz     r3,  (bitMenuCursorX - mainLoop)(r14)
    mulli   r3,  r3,  11
    addi    r3,  r3,  GAMEBIT_MENU_XPOS + 2 + (11*7)
    li      r4,  GAMEBIT_MENU_YPOS + 21
    li      r5,  16 # width
    li      r6,  16 # height
    bl      menuDrawBoxOpaque

    b       menuEndSub

gamebitMenu_doInput:
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)
    lhz     r17, (bitMenuIdx - mainLoop)(r14)

    # check the buttons
    bl      menuGetInput
    # r3=buttons, r4=stick X, r5=stick Y,
    # r6=CX, r7=CY, r8=L, r9=R
    andi.   r10, r3,  PAD_BUTTON_B
    bne     .gameBitMenu_close

    # check analog stick
    cmpwi   r5,   0x10
    bgt     .gameBitMenu_up
    cmpwi   r5,  -0x10
    blt     .gameBitMenu_down
    cmpwi   r4,   0x10
    bgt     .gameBitMenu_right
    cmpwi   r4,  -0x10
    blt     .gameBitMenu_left

    # check C stick - up/down/inc/dec with no delay
    cmpwi   r7,   0x10
    bgt     .gameBitMenu_up
    cmpwi   r7,  -0x10
    blt     .gameBitMenu_down
    cmpwi   r6,   0x10
    bgt     .gameBitMenu_inc
    cmpwi   r6,  -0x10
    blt     .gameBitMenu_dec

    # check L/R - jump by page
    cmpwi   r8,   0x04
    bgt     .gameBitMenu_prevPage
    cmpwi   r9,   0x04
    bgt     .gameBitMenu_nextPage

    # check X/Y - inc/dec
    andi.   r10, r3,  PAD_BUTTON_X
    bne     .gameBitMenu_incDigit
    andi.   r10, r3,  PAD_BUTTON_Y
    bne     .gameBitMenu_decDigit

    # check Z - reset to zero
    andi.   r10, r3,  PAD_BUTTON_Z
    bne     .gameBitMenu_clear

    b       menuEndSub

.gameBitMenu_up: # up pressed
    subi    r17, r17, 1
.gameBitMenu_up2:
    cmpwi   r17, 0
    bge     .gameBitMenu_storeIdx
    addi    r17, r17, MAX_GAMEBIT - 1

.gameBitMenu_down: # down pressed
    addi    r17, r17, 1
.gameBitMenu_down2:
    cmpwi   r17, MAX_GAMEBIT
    blt     .gameBitMenu_storeIdx
    li      r17, 0

.gameBitMenu_storeIdx:
    sth     r17, (bitMenuIdx - mainLoop)(r14)
    # don't use any delay if using C stick.
    andi.   r7,  r7,  0xF0 # deadzone
    bne     menuEndSub
.gameBitMenu_delayMoveAndEnd:
    li      r3,  MOVE_DELAY
    stb     r3,  (menuJustMoved - mainLoop)(r14)
    b       menuEndSub

.gameBitMenu_right:
    lbz     r16, (bitMenuCursorX - mainLoop)(r14)
    addi    r16, r16, 1
.gameBitMenu_storeCursor:
    andi.   r16, r16, 7
    stb     r16, (bitMenuCursorX - mainLoop)(r14)
    b       .gameBitMenu_delayMoveAndEnd

.gameBitMenu_left:
    lbz     r16, (bitMenuCursorX - mainLoop)(r14)
    subi    r16, r16, 1
    b       .gameBitMenu_storeCursor

.gameBitMenu_inc:
    andi.   r6,  r6,  0xF0 # deadzone
    bne     .gameBitMenu_inc_noDelay
    li      r3,  MOVE_DELAY
    stb     r3,  (menuJustMoved - mainLoop)(r14)
.gameBitMenu_inc_noDelay:
    mr      r3,  r17
    CALL    gameBitIncrement
    b       menuEndSub

.gameBitMenu_dec:
    andi.   r6,  r6,  0xF0 # deadzone
    bne     .gameBitMenu_dec_noDelay
    li      r3,  MOVE_DELAY
    stb     r3,  (menuJustMoved - mainLoop)(r14)
.gameBitMenu_dec_noDelay:
    mr      r3,  r17
    CALL    gameBitDecrement
    b       menuEndSub

.gameBitMenu_incDigit:
    li      r18, 1
    b       .gameBitMenu_adjustDigit

.gameBitMenu_decDigit:
    li      r18, -1

.gameBitMenu_adjustDigit:
    li      r3,  MOVE_DELAY
    stb     r3,  (menuJustMoved - mainLoop)(r14)

    mr      r3,  r17
    CALL    mainGetBit
    mr      r4,  r3
    # r4:  value to adjust
    # r16: digit to adjust (0=rightmost)
    # r18: amount to adjust by
    lbz     r16, (bitMenuCursorX - mainLoop)(r14)
    li      r5,  7
    sub     r16, r5,  r16 # 7 - x
    slwi    r16, r16, 2 # adjust hex digit

    # make a mask so that we can adjust one digit at a time
    # without affecting the others.
    li      r5,  0xF
    slw     r5,  r5,  r16 # mask
    slw     r18, r18, r16 # adjust correct digit

    li      r7,  -1
    xor     r8,  r5,  r7  # invert mask
    and     r8,  r8,  r4  # get only unchanged part
    add     r4,  r4,  r18 # do change
    and     r6,  r4,  r5  # get only changed part
    or      r4,  r8,  r6
    b       .gameBitMenu_set

.gameBitMenu_clear:
    li      r4,  0
.gameBitMenu_set:
    mr      r3,  r17
    CALL    mainSetBits
    b       menuEndSub

.gameBitMenu_prevPage:
    subi    r17, r17, 0x100
    b       .gameBitMenu_up2

.gameBitMenu_nextPage:
    addi    r17, r17, 0x100
    b       .gameBitMenu_down2

.gameBitMenu_close:
    bl      returnToMainMenu
    b       menuEndSub
