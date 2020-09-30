.ascii "heaplist" # 8 byte file ID for debug

.set HEAP_LIST_XPOS,    0
.set HEAP_LIST_YPOS,  120
.set HEAP_LIST_WIDTH, 635
.set HEAP_LIST_HEIGHT,100

heapList:
    # subroutine: runs the Heap List menu.
    # expects r14 = mainLoop.
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    #bl      menuHideHud
    bl      menuSetFixedWidth
    bl      heapList_doInput
    bl      heapList_Main
    b       menuEndSub


heapList_Main: # draw list of heaps.
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    # get the heap table
    LOAD    r15, heaps

    # draw the box
    li      r3,  HEAP_LIST_XPOS   # X
    li      r4,  HEAP_LIST_YPOS   # Y
    li      r5,  HEAP_LIST_WIDTH  # width
    li      r6,  HEAP_LIST_HEIGHT # height
    li      r20, 255 # opacity
    bl      menuDrawBox

    LOAD    r3,  0xFFFFFFFF
    bl      menuSetTextColor

    # draw the header
    addi    r3,  r14, fmt_heapListHeader - mainLoop
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  HEAP_LIST_XPOS + 8 # X pos
    li      r6,  HEAP_LIST_YPOS + 8 # Y pos
    CALL    gameTextShowStr

    li      r17, 0 # loop index
    lbz     r19, (whichHeap - mainLoop)(r14)
    li      r20, HEAP_LIST_YPOS + LINE_HEIGHT + 8 # string Y pos

.heapList_nextHeap:
    mulli   r4,  r17, 0x14
    add     r18, r4,  r15   # r18 = &Heap

    # set color
    li      r3,  255
    cmpw    r17, r19
    bne     .heapList_notSelected
    li      r3,  0
.heapList_notSelected:
    li      r4,  255
    li      r5,  255
    li      r6,  255
    CALL    gameTextSetColor

    # make line
    addi    r3,  r1,  SP_STR_BUF
    addi    r4,  r14, fmt_heapListEntry - mainLoop
    mr      r5,  r17       # index
    lwz     r6,  0x00(r18) # total bytes
    lwz     r7,  0x08(r18) # total blocks
    lwz     r8,  0x04(r18) # used bytes
    lwz     r9,  0x0C(r18) # used blocks
    lwz     r10, 0x10(r18) # data
    stw     r10, SP_ARG10(r1)
    sub     r10, r7,  r9 # free blocks
    stw     r10, SP_ARG9(r1)
    sub     r10, r6,  r8 # free bytes
    CALL    sprintf

    addi    r3,  r1,  SP_STR_BUF
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  HEAP_LIST_XPOS + 8  # X pos
    mr      r6,  r20 # Y pos
    CALL    gameTextShowStr

    # next line
    addi    r17, r17, 1
    cmpwi   r17, NUM_HEAPS # last heap?
    bge     menuEndSub
    addi    r20, r20, LINE_HEIGHT
    cmpwi   r20, HEAP_LIST_YPOS + HEAP_LIST_HEIGHT - LINE_HEIGHT
    blt     .heapList_nextHeap

    b       menuEndSub

heapList_doInput:
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)
    lbz     r17, (whichHeap - mainLoop)(r14)

    # check the buttons
    bl      menuGetInput
    # r3=buttons, r4=stick X, r5=stick Y,
    # r6=CX, r7=CY, r8=L, r9=R
    andi.   r10, r3,  PAD_BUTTON_B
    bne     .heapList_close
    andi.   r10, r3,  PAD_BUTTON_A
    bne     .heapList_open

    # check analog stick
    cmpwi   r5,   0x10
    bgt     .heapList_up
    cmpwi   r5,  -0x10
    blt     .heapList_down
    cmpwi   r4,   0x10
    b       menuEndSub

.heapList_up: # up pressed
    subi    r17, r17, 1
    cmpwi   r17, 0
    bge     .heapList_storeIdx
    addi    r17, r17, NUM_HEAPS - 1

.heapList_down: # down pressed
    addi    r17, r17, 1
    cmpwi   r17, NUM_HEAPS
    blt     .heapList_storeIdx
    li      r17, 0

.heapList_storeIdx:
    stb     r17, (whichHeap - mainLoop)(r14)
    li      r3,  MOVE_DELAY
    stb     r3,  (menuJustMoved - mainLoop)(r14)
    b       menuEndSub

.heapList_close:
    bl      returnToMainMenu
    b       menuEndSub

.heapList_open:
    li      r3,  0
    sth     r3,  (heapMenuIdx - mainLoop)(r14)
    li      r3,  MENU_ID_HEAP
    b       returnToMenu
