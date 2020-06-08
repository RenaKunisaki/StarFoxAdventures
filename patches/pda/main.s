mainLoop: # called from main loop. r3 = mainLoop
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r5
    stw   r5, SP_LR_SAVE(r1)
    stmw  r3, SP_GPR_SAVE(r1)
    mr    r14, r3

    # do HUD overrides, even if menu isn't open.
    bl    doHudOverrides

    # check if menu is closed or closing.
    lbz   r4, (menuVisible - mainLoop)(r14)
    cmpwi r4, 0
    bne   .menuMain
    beql  doCloseAnimation # if menu not "visible", do close anim.
    cmpwi r3, 0
    beq   .menuNotOpen # if close anim finished, don't draw anything.
    b     .drawMain

.menuMain:
    # ------ menu is visible ------
    # pause game
    LOADWH r5, playerLocked
    STOREB r4, playerLocked, r5 # stops all objects
    STOREB r4, pauseDisabled, r5 # inhibit pause menu
    li     r4, 1
    STOREB r4, shouldCloseCMenu, r5 # inhibt C menu

    # check input
    LOADWH  r5, controllerStates
    LOADWL2 r9, buttonsJustPressed, r5
    andi.   r9, r9, PAD_BUTTON_B
    bne     .close

    LOADBL2 r6, controllerStates+2, r5 # x
    LOADBL2 r7, controllerStates+3, r5 # y
    extsb   r6, r6 # sign extend (PPC Y U NO LBA OPCODE!?)
    extsb   r7, r7
    or      r8, r6, r7 # either
    andi.   r8, r8, 0xF8 # deadzone

    lbz     r5, (menuJustMoved - mainLoop)(r14)
    cmpwi   r5, 0
    beq     .checkMove
    # we already moved. don't move again until joystick is reset.
    cmpwi   r8, 0
    bne     .draw
    b       .resetJustMoved # r7 must be 0


.checkMove: # Check if joystick moved.
    cmpwi   r7, 0x10
    bgt     .up
    cmpwi   r7, -0x10
    blt     .down
    cmpwi   r6, 0x10
    bgt     .right
    cmpwi   r6, -0x10
    blt     .left

.resetJustMoved:
    stb     r7, (menuJustMoved - mainLoop)(r14)

.draw: # do animation and setup box
    bl     doOpenAnimation

.drawMain:
    # draw box and title
    addi   r3, r14, title - mainLoop # text
    li     r4, 0xC # box type
    li     r5, 0 # X pos
    li     r6, 0 # Y pos
    CALL  gameTextShowStr

    # draw menu
    addi  r15, r14, (itemDrawFuncs - mainLoop) - 4
    #li    r16, MENU_YPOS + LINE_HEIGHT # Y offset
    lhz   r16, 0x0E(r19)
    addi  r16, r16, LINE_HEIGHT # Y offset
    lbz   r17, (menuSelItem - mainLoop)(r14)
    addi  r18, r1, SP_STR_BUF

.nextItem:
    li    r3, 255
    li    r4, 255
    li    r5, 255
    cmpwi r17, 0
    bne   .drawItem

    lbz   r3, (menuSelColor - mainLoop)(r14)
    addi  r3, r3, 4
    stb   r3, (menuSelColor - mainLoop)(r14)
    #xori  r4, r3, 0xFF
    addi  r4, r3, 0x80

.drawItem:
    mr    r6, r20
    CALL  gameTextSetColor
    subi  r17, r17, 1
    lwzu  r3, 4(r15)
    cmpwi r3, 0
    beq   .mainEnd
    add   r3, r3, r14
    mtspr CTR, r3
    mr    r3, r18
    bctrl
    # the item draw function should return r4=string,
    # and any additional sprintf args.
    CALL sprintf
    addi  r3, r1, SP_STR_BUF
    li    r4, 0x93 # box type
    #li    r5, MENU_XPOS + 8 # X pos
    lhz   r5, 0x0C(r19) # X pos
    addi  r5, r5, 10
    mr    r6, r16 # Y pos
    addi  r16, r16, LINE_HEIGHT
    CALL  gameTextShowStr
    b     .nextItem


.menuNotOpen:
    # allow L+Z+B to open menu as well.
    LOADWH  r5, controllerStates
    LOADWL2 r6, buttonsJustPressed, r5
    LOADHL2 r7, controllerStates, r5 # buttons held
    andi.   r7, r7, PAD_BUTTON_L | PAD_BUTTON_Z
    cmpwi   r7, PAD_BUTTON_L | PAD_BUTTON_Z
    bne     .mainEnd
    cmpwi   r6, PAD_BUTTON_B
    bne     .mainEnd
    li      r4, 1
    stb     r4, (menuVisible - mainLoop)(r14)

.mainEnd:
    lwz  r5, SP_LR_SAVE(r1)
    mtlr r5 # restore LR
    lmw  r3, SP_GPR_SAVE(r1)
    addi r1, r1, STACK_SIZE # restore stack ptr
    blr


.close: # close the menu
    li     r3, 0
    stb    r3, (menuVisible - mainLoop)(r14)
    LOADWH r5, playerLocked
    STOREB r3, playerLocked, r5 # unpause game
    LOADWH r5, pauseDisabled
    STOREB r3, pauseDisabled, r5 # allow pause menu
    li     r4, 0x03F2
    CALL   audioPlaySound
    b      .mainEnd

.finishClose: # called once anim timer reaches 0
    # restore textbox config
    lwz    r19, (boxAddr - mainLoop)(r14)
    li     r4, 0x0186
    sth    r4, 0x00(r19) # width
    li     r4, 0x00C8
    sth    r4, 0x02(r19) # height
    li     r4, 0x0028
    sth    r4, 0x0C(r19) # X
    li     r4, 0x0032
    sth    r4, 0x0E(r19) # Y
    b      .mainEnd


.up: # up pressed
    lbz   r17, (menuSelItem - mainLoop)(r14)
    subi  r17, r17, 1
    cmpwi r17, 0
    bge   .storeSel

    # get max item
    li    r17, -1
    addi  r15, r14, (itemDrawFuncs - mainLoop) - 4
.upNext:
    lwzu  r3, 4(r15)
    cmpwi r3, 0
    beq   .storeSel
    addi  r17, r17, 1
    b     .upNext

.storeSel:
    stb   r17, (menuSelItem - mainLoop)(r14)
    li    r17, 1
    stb   r17, (menuJustMoved - mainLoop)(r14)
    li    r4, 0xF3

.doSound: # r4 = sound ID
    li    r3, 0
    CALL  audioPlaySound
    b     .draw

.down: # Down was pressed
    lbz   r17, (menuSelItem - mainLoop)(r14)
    addi  r17, r17, 1
    addi  r15, r14, (itemDrawFuncs - mainLoop)
    slwi  r16, r17, 2 # times 4
    lwzx  r3,  r15, r16
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
    lbz   r17, (menuSelItem - mainLoop)(r14)
    addi  r15, r14, (itemAdjustFuncs - mainLoop)
    slwi  r16, r17, 2 # times 4
    lwzx  r4,  r15, r16
    cmpwi r4,  0
    beq   .draw
    add   r4, r4, r14
    mtspr CTR, r4
    bctrl
    li    r17, 1
    stb   r17, (menuJustMoved - mainLoop)(r14)
    li    r4, 0xF3
    b     .doSound
