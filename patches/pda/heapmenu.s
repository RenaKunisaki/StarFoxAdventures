.set HEAP_MENU_XPOS,   70
.set HEAP_MENU_YPOS,   50
.set HEAP_MENU_WIDTH, 500
.set HEAP_MENU_HEIGHT,410

heapMenu:
    # subroutine: runs the Heap menu.
    # expects r14 = mainLoop.
    stwu   r1, -STACK_SIZE(r1) # get some stack space
    mflr   r0
    stw    r0, SP_LR_SAVE(r1)
    stmw   r13, SP_GPR_SAVE(r1)

    bl    menuHideHud

    # inhibit C menu
    li     r4, 1
    LOADWH r5, shouldCloseCMenu
    STOREB r4, shouldCloseCMenu, r5

    # get the heap table
    LOAD   r15, heaps
    lbz    r3,  (whichHeap - mainLoop)(r14)
    mulli  r3,  r3, 0x14
    add    r15, r15, r3
    lwz    r21, 0x0C(r15) # used blocks
    lwz    r22, 0x10(r15) # data

    bl     heapMenu_doInput
    bl     heapMenu_Main
    b      menuEndSub


heapMenu_Main: # draw list of entries.
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0, SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    # draw the box
    li      r3,  HEAP_MENU_XPOS   # X
    li      r4,  HEAP_MENU_YPOS   # Y
    li      r5,  HEAP_MENU_WIDTH  # width
    li      r6,  HEAP_MENU_HEIGHT # height
    li      r20, 255 # opacity
    bl      menuDrawBox

    # first item (selected) in blue
    LOAD    r3, 0x00FFFFFF
    bl      menuSetTextColor

    # draw the header
    addi    r3,  r14, fmt_heapMenuHeader - mainLoop
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  HEAP_MENU_XPOS + 8 # X pos
    li      r6,  HEAP_MENU_YPOS + 8 # Y pos
    CALL    gameTextShowStr

    lhz     r17, (heapMenuIdx - mainLoop)(r14)
    li      r20, HEAP_MENU_YPOS + LINE_HEIGHT + 8 # string Y pos

.heapMenu_nextItem:
    mulli   r3,  r17, 0x1C
    add     r18, r22, r3 # r18 = HeapEntry*
    lwz     r7,  0(r18) # pointer
    cmpwi   r7,  0
    beq     menuEndSub

    # draw first column: index
    addi    r3,  r1, SP_STR_BUF
    addi    r4,  r14, fmt_04X - mainLoop
    mr      r5,  r17 # idx
    CALL    sprintf
    addi    r3,  r1, SP_STR_BUF
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  HEAP_MENU_XPOS + 8  # X pos
    mr      r6,  r20 # Y pos
    CALL    gameTextShowStr

    # draw second column: tag
    addi    r3,  r1, SP_STR_BUF
    addi    r4,  r14, fmt_08X - mainLoop
    lwz     r5,  0x10(r18) # tag
    CALL    sprintf
    addi    r3,  r1, SP_STR_BUF
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  HEAP_MENU_XPOS + 8 + 60  # X pos
    mr      r6,  r20 # Y pos
    CALL    gameTextShowStr

    # draw third column: data
    addi    r3,  r1, SP_STR_BUF
    addi    r4,  r14, fmt_08X - mainLoop
    lwz     r5,  0(r18) # data
    CALL    sprintf
    addi    r3,  r1, SP_STR_BUF
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  HEAP_MENU_XPOS + 8 + 185  # X pos
    mr      r6,  r20 # Y pos
    CALL    gameTextShowStr

    # draw fourth column: size
    addi    r3,  r1, SP_STR_BUF
    addi    r4,  r14, fmt_08X - mainLoop
    lwz     r5,  0x04(r18) # size
    CALL    sprintf
    addi    r3,  r1, SP_STR_BUF
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  HEAP_MENU_XPOS + 8 + 310  # X pos
    mr      r6,  r20 # Y pos
    CALL    gameTextShowStr

    # next line
    LOAD    r3, 0xFFFFFFFF
    bl      menuSetTextColor
    addi    r17, r17, 1
    cmpw    r17, r21
    bge     menuEndSub
    addi    r20, r20, LINE_HEIGHT
    cmpwi   r20, HEAP_MENU_YPOS + HEAP_MENU_HEIGHT - LINE_HEIGHT
    blt     .heapMenu_nextItem

    b       menuEndSub

heapMenu_doInput:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0, SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)
    lhz     r17, (heapMenuIdx - mainLoop)(r14)

    # check the buttons
    bl      menuGetInput
    # r3=buttons, r4=stick X, r5=stick Y,
    # r6=CX, r7=CY, r8=L, r9=R
    andi.   r10, r3, PAD_BUTTON_B
    bne     .heapMenu_close

    # check analog stick
    cmpwi   r5, 0x10
    bgt     .heapMenu_up
    cmpwi   r5, -0x10
    blt     .heapMenu_down

    # check C stick - same as analog but no delay
    cmpwi   r7, 0x10
    bgt     .heapMenu_up
    cmpwi   r7, -0x10
    blt     .heapMenu_down

    # check L/R - extra fast scroll
    cmpwi   r8, 0x10
    bgt     .heapMenu_prevPage
    cmpwi   r9, 0x10
    bgt     .heapMenu_nextPage

    b       menuEndSub

.heapMenu_up: # up pressed
    subi    r17, r17, 1
.heapMenu_up2:
    cmpwi   r17, 0
    bge     .heapMenu_storeIdx
    add     r17, r17, r21
    subi    r17, r17, 1

.heapMenu_down: # down pressed
    addi    r17, r17, 1
.heapMenu_down2:
    cmpw    r17, r21
    blt     .heapMenu_storeIdx
    li      r17, 0

.heapMenu_storeIdx:
    sth     r17, (heapMenuIdx - mainLoop)(r14)
    # don't use any delay if using C stick.
    andi.   r7, r7, 0xF0 # deadzone
    bne     menuEndSub
    li      r3,  MOVE_DELAY
    stb     r3,  (menuJustMoved - mainLoop)(r14)
    b       menuEndSub

.heapMenu_prevPage:
    addi    r17, r17, -0x10
    b       .heapMenu_up2

.heapMenu_nextPage:
    addi    r17, r17, 0x10
    b       .heapMenu_down2

.heapMenu_close:
    li      r3, MENU_ID_HEAP_LIST
    bl      returnToMenu
    b       menuEndSub
