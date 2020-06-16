mainLoop: # called from main loop. r3 = mainLoop
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r5
    stw   r5, SP_LR_SAVE(r1)
    stmw  r13, SP_GPR_SAVE(r1)
    mr    r14, r3

    # do HUD overrides, even if menu isn't open.
    bl    doHudOverrides

    # check if the menu is open.
    lbz    r4, (menuVisible - mainLoop)(r14)
    cmpwi  r4, 0
    beq    .mainLoop_menuNotOpen
    #li     r4, 1
    STOREB r4, shouldCloseCMenu, r5 # inhibt C menu

.mainLoop_menuNotOpen:
    # we still run the menu routine, so it can do the close
    # animation. we must be on the main menu for the open
    # flag to be cleared, so don't worry about the other menus.
    lbz   r4, (whichMenu - mainLoop)(r14)
    slwi  r4, r4, 2
    addi  r5, r14, menuPtrs - mainLoop
    lwzx  r5, r4, r5
    add   r5, r5, r14
    mtspr CTR, r5
    bctrl

menuEndSub:
    # common end code for menu subroutines.
    lwz  r0, SP_LR_SAVE(r1)
    mtlr r0 # restore LR
    lmw  r13, SP_GPR_SAVE(r1)
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

menuPtrs: # menu main function pointers
    .int runMenu     - mainLoop
    .int objectMenu  - mainLoop
    .int gamebitMenu - mainLoop
    .int warpMenu    - mainLoop
    .int heapList    - mainLoop
    .int heapMenu    - mainLoop

returnToMainMenu:
    # called from other menus
    li   r3, MENU_ID_MAIN
returnToMenu: # r3 = which
    stb  r3, (whichMenu - mainLoop)(r14)
    li   r3, MOVE_DELAY
    stb  r3, (menuJustMoved - mainLoop)(r14)
    blr

runMenu:
    # subroutine: runs the menu logic.
    # expects r14 = mainLoop.
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r5
    stw   r5, SP_LR_SAVE(r1)
    stmw  r13, SP_GPR_SAVE(r1)

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
    bl     menuDraw
    b      menuEndSub

menuDraw:
    # subroutine: draws the menu.
    # expects r14 = mainLoop, r21 = draw funcs, r22 = adjust funcs
    stwu   r1, -STACK_SIZE(r1) # get some stack space
    mflr   r5
    stw    r5, SP_LR_SAVE(r1)
    stmw   r13, SP_GPR_SAVE(r1)

    # draw title
    lwz    r3, 0(r21) # get title string
    add    r3, r3, r14
    li     r4, 0 # box type: (center, y+???) with no background
    #lhz    r5, (menuBoxX - mainLoop)(r14) # X pos
    lhz    r6, (menuBoxY - mainLoop)(r14) # Y pos
    subi   r6, r6, 30 # offset to actually fit the box
    CALL   gameTextShowStr

    # setup text
    lhz    r16, (menuBoxY - mainLoop)(r14)
    addi   r16, r16, LINE_HEIGHT + 8 # text Y offset
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
    li     r4,  MENU_TEXTBOX_ID # box type
    lhz    r5,  (menuBoxX - mainLoop)(r14) # X pos
    addi   r5,  r5, 10
    mr     r6,  r16 # Y pos
    addi   r16, r16, LINE_HEIGHT
    CALL   gameTextShowStr
    b      .nextItem


menuGetInput:
    # subroutine: checks input.
    # expects r14 = mainLoop
    # returns r3=buttons, r4=stick X, r5=stick Y,
    # r6=CX, r7=CY, r8=L, r9=R
    # returns all inputs zero if menuJustMoved timer not zero.

    # are we doing open animation?
    lwz     r5, (menuAnimTimer - mainLoop)(r14)
    lis     r6, 0x3F80 # 1.0
    cmpw    r5, r6
    beq     .notOpening

    # ignore the inputs for a while.
.ignore:
    li      r5, 31
    b       .notNeutral

.notOpening:
    LOADHA r4, curGameText
    cmpwi  r4, 0
    bge    .ignore

    # get the controller state
    # r6 = stick X, r7 = stick Y
    LOADWH  r10, controllerStates
    LOADBL2 r6,  controllerStates+2, r10 # stick x
    LOADBL2 r7,  controllerStates+3, r10 # stick y
    LOADBL2 r8,  controllerStates+4, r10 # CX
    LOADWL2 r9,  buttonsJustPressed, r10
    extsb   r6,  r6 # sign extend (PPC Y U NO LBA OPCODE!?)
    extsb   r7,  r7
    extsb   r8,  r8

    # have we just moved?
    lbz     r5, (menuJustMoved - mainLoop)(r14)
    cmpwi   r5, 0
    beq     .notJustMoved

    # is the stick in a neutral position?
    or      r3, r6, r7 # either axis
    or      r3, r3, r8 # also C stick
    andi.   r3, r3, 0xF8 # deadzone
    bne     .notNeutral

    # and the L/R buttons?
    LOADBL2 r8, controllerStates+6, r10 # L
    LOADBL2 r9, controllerStates+7, r10 # R
    or      r3, r8, r9
    andi.   r3, r3, 0xFC # deadzone
    bne     .notNeutral

    # stick is neutral, so reset the move timer.
    li      r5, 1

.notNeutral: # set the move timer.
    subi    r5, r5, 1
    stb     r5, (menuJustMoved - mainLoop)(r14)

    # don't move this frame.
    li      r3, 0
    li      r4, 0
    li      r5, 0
    li      r6, 0
    li      r7, 0
    li      r8, 0
    li      r9, 0
    blr

.notJustMoved: # we didn't move recently
    mr      r3, r9 # buttons
    mr      r4, r6 # stick X
    mr      r5, r7 # stick Y
    mr      r6, r8 # CX
    LOADBL2 r7, controllerStates+5, r10 # CY
    extsb   r7, r7
    LOADBL2 r8, controllerStates+6, r10 # L
    LOADBL2 r9, controllerStates+7, r10 # R
    blr


menuHandleInput:
    # subroutine: runs the menu logic.
    # expects r14 = mainLoop, r21 = draw funcs, r22 = adjust funcs
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r5
    stw   r5, SP_LR_SAVE(r1)
    stmw  r13, SP_GPR_SAVE(r1)

    # pause game
    li     r4, 1
    LOADWH r5, playerLocked
    STOREB r4, playerLocked, r5 # stops all objects
    STOREB r4, pauseDisabled, r5 # inhibit pause menu
    # inhibit game timer
    LOAD   r5, 0x800140BC # gameTimerRun
    LOAD   r4, 0x4E800020 # blr
    stw    r4, 0(r5)
    li     r4, 0
    icbi   r4, r5 # flush instruction cache

    bl     menuGetInput
    # r3=buttons, r4=stick X, r5=stick Y,
    # r6=CX, r7=CY, r8=L, r9=R

    andi.   r10, r3, PAD_BUTTON_B
    bne     .close # B pressed -> close menu
    andi.   r10, r3, PAD_BUTTON_A
    bne     .select # A pressed -> select item

    lbz   r17, (menuPage - mainLoop)(r14)

    # check analog stick
    cmpwi   r5, 0x10 # stick up
    bgt     .up
    cmpwi   r5, -0x10 # stick down
    blt     .down
    cmpwi   r4, 0x10 # stick right
    bgt     .right
    cmpwi   r4, -0x10 # stick left
    blt     .left
    cmpwi   r6, 0x10 # C right
    bgt     .nextPage
    cmpwi   r6, -0x10 # C left
    blt     .prevPage
    cmpwi   r8, 0x04 # L
    bge     .leftFast
    cmpwi   r9, 0x04 # R
    blt     menuEndSub # no input

    # right fast
    #li    r3, 16
    srwi   r3, r9, 2
    b     .doAdjustNoDelay

.leftFast:
    #li    r3, -16
    srwi   r3, r8, 2
    neg    r3, r3
    b     .doAdjustNoDelay

.prevPage:
    addi  r17, r17, -1

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
    # reset cursor to first item
    li    r17, 0
    stb   r17, (menuSelItem - mainLoop)(r14)
    li    r4,  0xFC
    b     .doSound

.nextPage:
    addi  r17, r17, 1
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

.doAdjust:
    li    r17, MOVE_DELAY
    stb   r17, (menuJustMoved - mainLoop)(r14)
.doAdjustNoDelay:
    lbz   r17, (menuSelItem - mainLoop)(r14)
    #add   r15, r14, r22 # get adjust func
    slwi  r16, r17, 2 # times 4
    lwzx  r8,  r22, r16
    cmpwi r8,  0
    beq   menuEndSub # no adjust func
    add   r8, r8, r14
    mtspr CTR, r8
    li    r9, 0xF3 # sound effect ID
    bctrl # call it
    mr    r4, r9
    b     .doSound

.select: # A was pressed
    li     r3, 0
    b      .doAdjust

.close: # close the menu
    li     r3, 0
    stb    r3, (menuVisible - mainLoop)(r14)
    stb    r3, (whichMenu - mainLoop)(r14)
    LOADWH r5, playerLocked
    STOREB r3, playerLocked, r5 # unpause game
    LOADWH r5, pauseDisabled
    STOREB r3, pauseDisabled, r5 # allow pause menu

    # restore game timer
    LOAD   r5, 0x800140BC # gameTimerRun
    LOAD   r4, 0x9421FFC0 # original opcode
    stw    r4, 0(r5)
    li     r4, 0
    icbi   r4, r5 # flush instruction cache

    # restore HUD
    lbz     r4, (menuWasHudHidden - mainLoop)(r14)
    LOADWH  r3, hudHidden
    STOREB  r4, hudHidden, r3
    lbz     r4, (menuWasPdaOn - mainLoop)(r14)
    LOADWH  r3, pdaOn
    STOREB  r4, pdaOn, r3

    li     r4, 0x03F2
    b      .doSound
