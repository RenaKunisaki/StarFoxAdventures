.set GAMEBIT_MENU_XPOS,  160
.set GAMEBIT_MENU_YPOS,   70
.set GAMEBIT_MENU_WIDTH, 300
.set GAMEBIT_MENU_HEIGHT,300
.set MAX_GAMEBIT,0x0F58

gamebitMenu:
    # subroutine: runs the GameBits menu.
    # expects r14 = mainLoop.
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r0
    stw   r0, SP_LR_SAVE(r1)
    stmw  r13, SP_GPR_SAVE(r1)

    # inhibit C menu
    li     r4, 1
    LOADWH r5, shouldCloseCMenu
    STOREB r4, shouldCloseCMenu, r5

    #bl    menuHideHud
    bl     gamebitMenu_doInput
    bl     gamebitMenu_Main
    b      menuEndSub


gamebitMenu_Main: # draw list of bits.
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r0
    stw   r0, SP_LR_SAVE(r1)
    stmw  r13, SP_GPR_SAVE(r1)

    # get the bit table
    LOADW r15, 0x803dcadc
    cmpwi r15, 0
    beq   menuEndSub

    # draw the box
    li      r3, GAMEBIT_MENU_XPOS   # X
    li      r4, GAMEBIT_MENU_YPOS   # Y
    li      r5, GAMEBIT_MENU_WIDTH  # width
    li      r6, GAMEBIT_MENU_HEIGHT # height
    li      r20, 255 # opacity
    bl      menuDrawBox

    # first item (selected) in blue
    LOAD    r3, 0x00FFFFFF
    bl      menuSetTextColor

    # draw the header
    addi    r3,  r14, fmt_bitListHeader - mainLoop
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  GAMEBIT_MENU_XPOS + 8 # X pos
    li      r6,  GAMEBIT_MENU_YPOS + 8 # Y pos
    CALL    gameTextShowStr

    lhz     r17, (bitMenuIdx - mainLoop)(r14)
    li      r20, GAMEBIT_MENU_YPOS + LINE_HEIGHT + 8 # string Y pos

.gameBitMenu_nextBit:
    slwi    r4,  r17, 2
    lwzx    r18, r4, r15   # r18 = bit def

    # get value
    mr      r3, r17
    CALL    mainGetBit
    mr      r10, r3

    # make line
    addi    r3,  r1, SP_STR_BUF
    addi    r4,  r14, fmt_bitListEntry - mainLoop
    mr      r5,  r17 # bit idx
    rlwinm  r6,  r18, 32-14, 0x0003 # table idx
    rlwinm  r7,  r18, 32-16, 0xFFFF # offset
    rlwinm  r8,  r18, 32- 8, 0x001F # size
    addi    r8,  r8, 1
    rlwinm  r9,  r18, 32-32, 0x00FF # unk03
    rlwinm  r11, r18, 32-13, 0x0001 # unk13
    slwi    r11, r11, 8
    or      r9,  r9, r11
    CALL    sprintf

    addi    r3,  r1, SP_STR_BUF
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  GAMEBIT_MENU_XPOS + 8  # X pos
    mr      r6,  r20 # Y pos
    CALL    gameTextShowStr

    LOAD    r3, 0xFFFFFFFF
    bl      menuSetTextColor

    # next line
    addi    r17, r17, 1
    cmpwi   r17, MAX_GAMEBIT # last bit?
    bge     menuEndSub
    addi    r20, r20, LINE_HEIGHT
    cmpwi   r20, GAMEBIT_MENU_YPOS + GAMEBIT_MENU_HEIGHT - LINE_HEIGHT
    blt     .gameBitMenu_nextBit

    b       menuEndSub

gamebitMenu_doInput:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0, SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)
    lhz     r17, (bitMenuIdx - mainLoop)(r14)

    # check the buttons
    bl      menuGetInput
    # r3=buttons, r4=stick X, r5=stick Y,
    # r6=CX, r7=CY, r8=L, r9=R
    andi.   r10, r3, PAD_BUTTON_B
    bne     .gameBitMenu_close

    # check analog stick
    cmpwi   r5, 0x10
    bgt     .gameBitMenu_up
    cmpwi   r5, -0x10
    blt     .gameBitMenu_down
    cmpwi   r4, 0x10
    bgt     .gameBitMenu_inc
    cmpwi   r4, -0x10
    blt     .gameBitMenu_dec

    # check C stick - same as analog but no delay
    cmpwi   r7, 0x10
    bgt     .gameBitMenu_up
    cmpwi   r7, -0x10
    blt     .gameBitMenu_down
    cmpwi   r6, 0x10
    bgt     .gameBitMenu_inc
    cmpwi   r6, -0x10
    blt     .gameBitMenu_dec

    # check L/R - extra fast scroll
    cmpwi   r8, 0x10
    bgt     .gameBitMenu_prevPage
    cmpwi   r9, 0x10
    bgt     .gameBitMenu_nextPage

    b       menuEndSub

.gameBitMenu_up: # up pressed
    subi    r17, r17, 1
    cmpwi   r17, 0
    bge     .gameBitMenu_storeIdx
    addi    r17, r17, MAX_GAMEBIT - 2
    # by adding -2 there and then falling into the Down
    # code, which adds 1, we avoid a branch.

.gameBitMenu_down: # down pressed
    addi    r17, r17, 1
    cmpwi   r17, MAX_GAMEBIT
    blt     .gameBitMenu_storeIdx
    li      r17, 0

.gameBitMenu_storeIdx:
    sth     r17, (bitMenuIdx - mainLoop)(r14)
    # don't use any delay if using C stick.
    andi.   r7, r7, 0xF0 # deadzone
    bne     menuEndSub
    li      r3,  MOVE_DELAY
    stb     r3,  (menuJustMoved - mainLoop)(r14)
    b       menuEndSub

.gameBitMenu_inc:
    andi.   r6, r6, 0xF0 # deadzone
    bne     .gameBitMenu_inc_noDelay
    li      r3,  MOVE_DELAY
    stb     r3,  (menuJustMoved - mainLoop)(r14)
.gameBitMenu_inc_noDelay:
    mr      r3, r17
    CALL    gameBitIncrement
    b       menuEndSub

.gameBitMenu_dec:
    andi.   r6, r6, 0xF0 # deadzone
    bne     .gameBitMenu_dec_noDelay
    li      r3,  MOVE_DELAY
    stb     r3,  (menuJustMoved - mainLoop)(r14)
.gameBitMenu_dec_noDelay:
    mr      r3, r17
    CALL    gameBitDecrement
    b       menuEndSub

.gameBitMenu_prevPage:
    addi    r17, r17, -0x10
.gameBitMenu_setPage:
    sth     r17, (bitMenuIdx - mainLoop)(r14)
    b       menuEndSub

.gameBitMenu_nextPage:
    addi    r17, r17, 0x10
    b       .gameBitMenu_setPage

.gameBitMenu_close:
    bl      returnToMainMenu
    b       menuEndSub
