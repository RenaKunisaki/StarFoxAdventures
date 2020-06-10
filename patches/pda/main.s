mainLoop: # called from main loop. r3 = mainLoop
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r5
    stw   r5, SP_LR_SAVE(r1)
    stmw  r3, SP_GPR_SAVE(r1)
    mr    r14, r3

    # do HUD overrides, even if menu isn't open.
    bl    doHudOverrides
    bl    runMenu

menuEndSub:
    # common end code for menu subroutines.
    lwz  r5, SP_LR_SAVE(r1)
    mtlr r5 # restore LR
    lmw  r3, SP_GPR_SAVE(r1)
    addi r1, r1, STACK_SIZE # restore stack ptr
    blr

checkMenuOpenKey:
    # allow L+Z+B to open menu as well.
    LOADWH  r5, controllerStates
    LOADWL2 r6, buttonsJustPressed, r5
    LOADHL2 r7, controllerStates, r5 # buttons held
    andi.   r7, r7, PAD_BUTTON_L | PAD_BUTTON_Z
    cmpwi   r7, PAD_BUTTON_L | PAD_BUTTON_Z
    bne     menuEndSub
    cmpwi   r6, PAD_BUTTON_B
    bne     menuEndSub
    li      r4, 1
    stb     r4, (menuVisible - mainLoop)(r14)
    b       menuEndSub

runMenu:
    # subroutine: runs the menu logic.
    # expects r14 = mainLoop.
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r5
    stw   r5, SP_LR_SAVE(r1)
    stmw  r3, SP_GPR_SAVE(r1)

    # get current page, and get pointers to the
    # draw function tables and adjust function tables.
    lbz   r3, (menuPage - mainLoop)(r14)
    slwi  r3, r3, 3 # r3 *= 8
    addi  r4, r14, menuPages - mainLoop
    add   r3, r3, r4
    lwz   r21, 0(r3) # draw funcs
    lwz   r22, 4(r3) # adjust funcs
    add   r21, r21, r14
    add   r22, r22, r14

    # check if the menu is open.
    lbz   r5, (menuVisible - mainLoop)(r14)
    cmpwi r5, 0
    bne   .menuIsOpen

    # menu isn't visible. do we need to run close anim?
    beql  doCloseAnimation
    cmpwi r3, 0
    beq   checkMenuOpenKey # the menu is completely hidden.
    # else, the close animation is still running,
    # so we still need to draw, but not respond to input.
    b .menuClosing

.menuIsOpen:
    bl     doOpenAnimation
    bl     menuHandleInput

.menuClosing:
    bl menuDraw
    b  menuEndSub


menuDraw:
    # subroutine: draws the menu.
    # expects r14 = mainLoop, r21 = draw funcs, r22 = adjust funcs
    stwu   r1, -STACK_SIZE(r1) # get some stack space
    mflr   r5
    stw    r5, SP_LR_SAVE(r1)
    stmw   r3, SP_GPR_SAVE(r1)

    # draw title
    lwz    r3, 0(r21) # get title string
    add    r3, r3, r14
    li     r4, 0xC # box type
    li     r5, 0 # X pos
    li     r6, 0 # Y pos
    CALL   gameTextShowStr

    # setup box
    lwz    r19, (boxAddr - mainLoop)(r14)
    lhz    r16, 0x0E(r19) # box Y pos
    addi   r16, r16, LINE_HEIGHT # text Y offset
    lbz    r17, (menuSelItem - mainLoop)(r14)
    addi   r18, r1, SP_STR_BUF

.nextItem:
    li    r3, 255
    li    r4, 255
    li    r5, 255
    cmpwi r17, 0
    bne   .drawItem

    # set the color for the selected item
    # and animate it
    lbz   r3, (menuSelColor - mainLoop)(r14)
    addi  r3, r3, 4
    stb   r3, (menuSelColor - mainLoop)(r14)
    #xori  r4, r3, 0xFF
    addi  r4, r3, 0x80

.drawItem:
    mr    r6, r20 # set by doAnimation
    CALL  gameTextSetColor
    subi  r17, r17, 1
    lwzu  r3, 4(r21) # get draw func
    cmpwi r3, 0
    beq   menuEndSub

    # stuff a placeholder into the string buffer.
    # something 3 bytes long that's easily recognized
    # and unlikely to appear in the game normally.
    LOAD  r4, 0x46555800 # "FUX"
    stw   r4, SP_STR_BUF(r1)

    add   r3, r3, r14
    mtspr CTR, r3
    mr    r3, r18
    bctrl # call draw func

    # the item draw function should return r4=string,
    # and any additional sprintf args.
    CALL   sprintf
    lwz    r19, (boxAddr - mainLoop)(r14)
    addi   r3,  r1, SP_STR_BUF
    li     r4,  0x93 # box type
    lhz    r5,  0x0C(r19) # X pos
    addi   r5,  r5, 10
    mr     r6,  r16 # Y pos
    addi   r16, r16, LINE_HEIGHT
    CALL   gameTextShowStr
    b      .nextItem


menuHandleInput:
    # subroutine: runs the menu logic.
    # expects r14 = mainLoop, r21 = draw funcs, r22 = adjust funcs
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r5
    stw   r5, SP_LR_SAVE(r1)
    stmw  r3, SP_GPR_SAVE(r1)

    # pause game
    li     r4, 1
    LOADWH r5, playerLocked
    STOREB r4, playerLocked, r5 # stops all objects
    STOREB r4, pauseDisabled, r5 # inhibit pause menu
    #li     r4, 1
    STOREB r4, shouldCloseCMenu, r5 # inhibt C menu

    # get the controller state
    # r6 = stick X, r7 = stick Y
    LOADWH  r5, controllerStates
    LOADBL2 r6, controllerStates+2, r5 # stick x
    LOADBL2 r7, controllerStates+3, r5 # stick y
    LOADBL2 r8, controllerStates+4, r5 # CX
    LOADWL2 r9, buttonsJustPressed, r5
    extsb   r6, r6 # sign extend (PPC Y U NO LBA OPCODE!?)
    extsb   r7, r7
    extsb   r8, r8

    # have we just moved?
    lbz     r5, (menuJustMoved - mainLoop)(r14)
    cmpwi   r5, 0
    beq     .notJustMoved

    # is the stick in a neutral position?
    or      r3, r6, r7 # either axis
    or      r3, r3, r8 # also C stick
    andi.   r3, r3, 0xF8 # deadzone
    bne     .stickNotNeutral

    # stick is neutral, so reset the move timer.
    li      r5, 1

.stickNotNeutral: # set the move timer.
    subi    r5, r5, 1
    stb     r5, (menuJustMoved - mainLoop)(r14)
    b menuEndSub # don't move this frame.


.notJustMoved: # we didn't move recently
    andi.   r3, r9, PAD_BUTTON_B
    bne     .close # B pressed -> close menu

    lbz   r17, (menuPage - mainLoop)(r14)

    # check analog stick
    cmpwi   r7, 0x10
    bgt     .up
    cmpwi   r7, -0x10
    blt     .down
    cmpwi   r6, 0x10
    bgt     .right
    cmpwi   r6, -0x10
    blt     .left
    cmpwi   r8, 0x10
    bgt     .prevPage
    cmpwi   r8, -0x10
    bge     menuEndSub # no input

    # next page
    addi  r17, r17, 1


.checkPage:
    slwi  r3, r17, 3 # r3 = r17 * 8
    addi  r4, r14, menuPages - mainLoop
    add   r3, r3, r4
    lwz   r21, 0(r3) # draw funcs
    cmpwi r21, 0
    bne   .notLastPage
    li    r17, 0 # wrap to first

.notLastPage:
    cmpwi r17, 0
    bge   .notFirstPage

    # scan to last page
    li    r17, -1
.scanNextPage:
    lwz   r3,  0(r4) # get draw func table
    addi  r4,  r4, 8
    cmpwi r3,  0
    beq   .notFirstPage
    addi  r17, r17, 1
    b     .scanNextPage

.notFirstPage:
    stb   r17, (menuPage - mainLoop)(r14)
    li    r17, MOVE_DELAY
    stb   r17, (menuJustMoved - mainLoop)(r14)
    li    r4,  0xFC
    b     .doSound

.prevPage:
    subi  r17, r17, 1
    b     .checkPage


.up: # up pressed
    lbz   r17, (menuSelItem - mainLoop)(r14)
    subi  r17, r17, 1
    cmpwi r17, 0
    bge   .storeSel

    # get max item
    li    r17, -1
    mr    r15, r21 # r15 = r21 - 4 for lwzu, +4 to skip title
.upNext:
    lwzu  r3, 4(r15)
    cmpwi r3, 0
    beq   .storeSel
    addi  r17, r17, 1
    b     .upNext

.storeSel:
    stb   r17, (menuSelItem - mainLoop)(r14)
    li    r17, MOVE_DELAY
    stb   r17, (menuJustMoved - mainLoop)(r14)
    li    r4, 0xF3

.doSound: # r4 = sound ID
    li    r3, 0
    CALL  audioPlaySound
    b menuEndSub


.down: # Down was pressed
    lbz   r17, (menuSelItem - mainLoop)(r14)
    addi  r17, r17, 1
    slwi  r16, r17, 2 # times 4
    addi  r16, r16, 4 # skip title
    # XXX this still allows us to select one item past the end. why?
    lwzx  r3,  r21, r16
    cmpwi r3,  0
    bne   .storeSel
    li    r17, 0
    b     .storeSel

.left: # Left was pressed
    li    r3, -1
    b     .doAdjust

.right: # Right was pressed
    li    r3, 1

.doAdjust: # 81682ce0
    lbz   r17, (menuSelItem - mainLoop)(r14)
    #add   r15, r14, r22 # get adjust func
    slwi  r16, r17, 2 # times 4
    lwzx  r4,  r22, r16
    cmpwi r4,  0
    beq   .draw # no adjust func
    add   r4, r4, r14
    mtspr CTR, r4
    bctrl # call it
    li    r17, MOVE_DELAY
    stb   r17, (menuJustMoved - mainLoop)(r14)
    li    r4, 0xF3
    b     .doSound

.close: # close the menu
    li     r3, 0
    stb    r3, (menuVisible - mainLoop)(r14)
    LOADWH r5, playerLocked
    STOREB r3, playerLocked, r5 # unpause game
    LOADWH r5, pauseDisabled
    STOREB r3, pauseDisabled, r5 # allow pause menu
    li     r4, 0x03F2
    b      .doSound
