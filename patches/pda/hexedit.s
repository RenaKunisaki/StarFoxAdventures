.ascii "hexedit " # 8 byte file ID for debug

.set HEXEDIT_XPOS,   30
.set HEXEDIT_YPOS,  100
.set HEXEDIT_WIDTH, 560
.set HEXEDIT_HEIGHT,328
.set HEXEDIT_NUM_COLS,8

hexeditMain:
    # subroutine: runs the Hex Edit menu.
    # expects r14 = mainLoop.
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    #bl     menuHideHud
    bl      menuSetFixedWidth
    bl      hexedit_doInput
    bl      hexedit_Main
    b       menuEndSub


hexedit_Main: # draw data
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    # r15: src address
    # r16: column
    # r18: str buf write pos
    # r19: str buf write pos for ASCII
    # r20: text Y pos

    # draw the box
    li      r3,  HEXEDIT_XPOS   # X
    li      r4,  HEXEDIT_YPOS   # Y
    li      r5,  HEXEDIT_WIDTH  # width
    li      r6,  HEXEDIT_HEIGHT # height
    li      r20, 255 # opacity
    bl      menuDrawBox

    LOAD    r3,  0xFFFFFFFF
    bl      menuSetTextColor

    # draw the header
    addi    r3,  r1, SP_STR_BUF
    addi    r4,  r14, s_hexEditHeader - mainLoop
    lwz     r5,  (hexEditAddr - mainLoop)(r14)
    CALL    sprintf

    addi    r3,  r1, SP_STR_BUF
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  HEXEDIT_XPOS + 8 # X pos
    li      r6,  HEXEDIT_YPOS + 8 # Y pos
    CALL    gameTextShowStr

    # draw instructions
    addi    r3,  r14, s_hexEditInstrs - mainLoop
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  HEXEDIT_XPOS + 8 # X pos
    li      r6,  HEXEDIT_YPOS + 8 + (HEXEDIT_HEIGHT-22) # Y pos
    CALL    gameTextShowStr

    lwz     r15, (hexEditAddr - mainLoop)(r14)
    subi    r15, r15, 1 # for lbzu
    li      r20, HEXEDIT_YPOS + LINE_HEIGHT + 8 # string Y pos

.hexedit_nextLine:
    li      r16, 0 # column
    # add address
    addi    r3,  r1,  SP_STR_BUF
    addi    r4,  r14, fmt_04X - mainLoop
    addi    r5,  r15, 1
    andi.   r5,  r5,  0xFFFF
    CALL    sprintf
    addi    r18, r1,  SP_STR_BUF+3 # 4, -1 for stbu
    addi    r19, r1,  SP_STR_BUF+3 + (HEXEDIT_NUM_COLS*3) + 1 # for text

.hexedit_nextByte:
    li      r3,  0x20
    stbu    r3,  1(r18)
    lbzu    r5,  1(r15)
    srwi    r3,  r5,  4 # high digit
    addi    r3,  r3,  0x30 # to ASCII
    cmpwi   r3,  0x39
    ble     .hexedit_highdigit_num
    addi    r3,  r3,  7
.hexedit_highdigit_num:
    stbu    r3,  1(r18)
    andi.   r3,  r5,  0xF # low digit
    addi    r3,  r3,  0x30 # to ASCII
    cmpwi   r3,  0x39
    ble     .hexedit_lowdigit_num
    addi    r3,  r3,  7
.hexedit_lowdigit_num:
    stbu    r3,  1(r18)
    cmpwi   r5,  0x20
    blt     .hexedit_printByte_notAscii
    cmpwi   r5,  0x7E
    ble     .hexedit_printByte_doPrint
.hexedit_printByte_notAscii:
    li      r5,  0x2E # period
.hexedit_printByte_doPrint:
    stbu    r5,  1(r19)
    addi    r16, r16, 1 # next column
    cmpwi   r16, HEXEDIT_NUM_COLS
    blt     .hexedit_nextByte
    li      r3,  0x20
    stb     r3,  1(r18) # space before text
    li      r3,  0
    stb     r3,  1(r19) # terminate string

    # print line
    addi    r3,  r1, SP_STR_BUF
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  HEXEDIT_XPOS + 8  # X pos
    mr      r6,  r20 # Y pos
    CALL    gameTextShowStr

    # next line
    addi    r20, r20, LINE_HEIGHT
    cmpwi   r20, HEXEDIT_YPOS + HEXEDIT_HEIGHT - LINE_HEIGHT
    blt     .hexedit_nextLine

.hexedit_drawCursor:
    # draw box at cursor position
    lbz     r3,  (hexEditCursorY - mainLoop)(r14)
    mulli   r4,  r3,  LINE_HEIGHT
    addi    r4,  r4,  HEXEDIT_YPOS + 4 # r4: Y pos

    lbz     r6,  (hexEditCursorX - mainLoop)(r14)
    mulli   r5,  r6,  11
    addi    r5,  r5,  HEXEDIT_XPOS + 2 + (5*11) # r5: X pos

    cmpwi   r3,  0 # first line?
    beq     .hexedit_drawCursor_firstLine

    srwi    r7,  r6,  1 # byte idx
    mulli   r7,  r7,  11 # skip spaces
    add     r5,  r5,  r7

.hexedit_drawCursor_firstLine:
    mr      r3,  r5
    li      r5,  16 # width
    li      r6,  16 # height
    bl      menuDrawBoxOpaque
    b       menuEndSub

hexedit_doInput:
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)
    lbz     r16, (hexEditCursorX - mainLoop)(r14)
    lbz     r17, (hexEditCursorY - mainLoop)(r14)
    lwz     r18, (hexEditAddr    - mainLoop)(r14)

    # check the buttons
    bl      menuGetInput
    # r3=buttons, r4=stick X, r5=stick Y,
    # r6=CX, r7=CY, r8=L, r9=R
    andi.   r10, r3,  PAD_BUTTON_B
    bne     .hexedit_close

    # check analog stick
    cmpwi   r5,   0x10
    bgt     .hexedit_up
    cmpwi   r5,  -0x10
    blt     .hexedit_down
    cmpwi   r4,   0x10
    bgt     .hexedit_right
    cmpwi   r4,  -0x10
    blt     .hexedit_left

    # check C stick - scroll fast
    cmpwi   r7,   0x10
    bgt     .hexedit_scrollUp
    cmpwi   r7,  -0x10
    blt     .hexedit_scrollDown

    # check L/R - jump by page
    #cmpwi   r8,   0x04
    #bgt     .hexedit_prevPage
    #cmpwi   r9,   0x04
    #bgt     .hexedit_nextPage

    # check X/Y - inc/dec
    andi.   r10, r3,  PAD_BUTTON_X
    bne     .hexedit_incDigit
    andi.   r10, r3,  PAD_BUTTON_Y
    bne     .hexedit_decDigit
    andi.   r10, r3,  PAD_BUTTON_MENU
    bne     .hexedit_followPtr

    b       menuEndSub

.hexedit_up: # up pressed
    subi    r17, r17, 1
    cmpwi   r17, 0
    bge     .hexedit_delayMoveAndEnd
    # scroll up
    li      r17,  0
    subi    r18, r18, HEXEDIT_NUM_COLS
    b       .hexedit_delayMoveAndEnd

.hexedit_down: # down pressed
    addi    r17, r17, 1
    cmpwi   r17, (HEXEDIT_HEIGHT / LINE_HEIGHT) - 2
    blt     .hexedit_delayMoveAndEnd
    # scroll down
    subi    r17, r17, 1
    addi    r18, r18, HEXEDIT_NUM_COLS

.hexedit_delayMoveAndEnd:
    li      r3,  MOVE_DELAY
    stb     r3,  (menuJustMoved  - mainLoop)(r14)
.hexedit_inputEnd:
    stb     r16, (hexEditCursorX - mainLoop)(r14)
    stb     r17, (hexEditCursorY - mainLoop)(r14)
    stw     r18, (hexEditAddr    - mainLoop)(r14)
    b       menuEndSub

.hexedit_right:
    addi    r16, r16, 1
    cmpwi   r16, HEXEDIT_NUM_COLS * 2
    blt     .hexedit_delayMoveAndEnd
    li      r16, 0
    b       .hexedit_delayMoveAndEnd

.hexedit_left:
    subi    r16, r16, 1
    cmpwi   r16, 0
    bge     .hexedit_delayMoveAndEnd
    li      r16, (HEXEDIT_NUM_COLS * 2) - 1
    b       .hexedit_delayMoveAndEnd

.hexedit_incDigit:
    li      r19, 1
    b       .hexedit_adjustDigit

.hexedit_decDigit:
    li      r19, -1

.hexedit_adjustDigit:
    # r4:  value to adjust
    # r19: amount to adjust by
    cmpwi   r17, 0 # first line?
    beq     .hexedit_adjustDigitAddr
    srwi    r3,  r16, 1
    add     r6,  r3,  r18 # address to edit
    mulli   r3,  r17, HEXEDIT_NUM_COLS
    subi    r3,  r3,  HEXEDIT_NUM_COLS
    add     r6,  r6,  r3
    lbz     r3,  0(r6)
    andi.   r0,  r16, 1 # which digit is cursor on?
    beq     .hexedit_adjustHighDigit

    # adjust low digit
    add     r4,  r3,  r19
    andi.   r4,  r4,  0x0F
    andi.   r5,  r3,  0xF0
    b       .hexEdit_adjustStore

.hexedit_adjustHighDigit:
    slwi    r19, r19, 4
    add     r4,  r3,  r19
    andi.   r4,  r4,  0xF0
    andi.   r5,  r3,  0x0F

.hexEdit_adjustStore:
    or      r3,  r4,  r5
    stb     r3,  0(r6)
    b       .hexedit_delayMoveAndEnd

.hexedit_adjustDigitAddr:
    li      r5,  7
    sub     r4,  r5,  r16 # 7 - x
    slwi    r4,  r4,  2 # adjust hex digit

    # make a mask so that we can adjust one digit at a time
    # without affecting the others.
    li      r5,  0xF
    slw     r5,  r5,  r4  # mask
    slw     r20, r19, r4  # adjust correct digit

    li      r7,  -1
    xor     r8,  r5,  r7  # invert mask
    and     r8,  r8,  r18 # get only unchanged part
    add     r18, r18, r20 # do change
    and     r6,  r18, r5  # get only changed part
    or      r18, r8,  r6
    b       .hexedit_delayMoveAndEnd

.hexedit_scrollUp:
    subi    r18, r18, HEXEDIT_NUM_COLS
    b       .hexedit_inputEnd

.hexedit_scrollDown:
    addi    r18, r18, HEXEDIT_NUM_COLS
    b       .hexedit_inputEnd

.hexedit_followPtr:
    srwi    r3,  r16, 1
    add     r6,  r3,  r18 # address to edit
    mulli   r3,  r17, HEXEDIT_NUM_COLS
    subi    r3,  r3,  HEXEDIT_NUM_COLS
    add     r6,  r6,  r3
    LOAD    r4,  0xFFFFFFFC
    and     r6,  r6,  r4 # ensure alignment
    lwz     r3,  0(r6)
    srwi    r4,  r3,  24
    cmpwi   r4,  0x80
    blt     .hexedit_inputEnd
    mr      r18, r3
    b       .hexedit_delayMoveAndEnd

.hexedit_close:
    bl      returnToMainMenu
    b       menuEndSub
