mainLoop: # called from main loop. r3 = mainLoop
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r5
    stw   r5, SP_LR_SAVE(r1)
    stmw  r3, SP_GPR_SAVE(r1)
    mr    r14, r3

    # do minimap size/opacity override
    LOADWH r5, minimapWidth
    lbz    r4, (minimapAlphaOverride - mainLoop)(r14)
    cmpwi  r4, 0xFF
    beq    .noAlphaOverride
    STOREH r4, minimapAlpha, r5

.noAlphaOverride:
    li      r4, 100 # default height
    LOADBL2 r6, minimapMode, r5
    cmpwi   r6, 0 # map mode?
    bne     .noMapOverride # don't change size
    lhz     r6, (minimapSizeOverride - mainLoop)(r14)
    cmpwi   r6, 0
    beq     .noMapOverride
    mr      r4, r6
    STOREW  r4, minimapWidth, r5

.noMapOverride:
    STOREW r4, minimapHeight, r5
    LOAD   r5, 0x801324CA # some instructions to patch for height
    sth    r4, 0(r5)
    sth    r4, 8(r5)
    li     r4, 0
    icbi   r4, r5 # flush instruction cache

    # 8013266A = 01B8, -> 01D8 to move map down
    # to move left, all following must be changed:
    # 8013267A = 0032 (the box)
    # 801326A6 = 0032 (the box clip)
    # 80132A92, 80132A96 (some texture shit)
    # 803e2210 (float) the clip offset
    # 80132A94 could just be changed to "add r5, r28, r3" unless a branch points directly at it
    # this also doesn't affect the D-pad icon and arrows.
    # also with huge map, it shudders sometimes, trying to shrink
    # back to normal size.

    lbz   r4, (menuVisible - mainLoop)(r14)
    cmpwi r4, 0
    beq   .closeAnim

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
    lwz    r3, (menuAnimTimer - mainLoop)(r14)
    cmpwi  r3, 0
    bne    .noOpenSound
    li     r4, 0x03E5
    CALL   audioPlaySound

.noOpenSound:
    # do animation.
    lfs    f1, (menuAnimTimer - mainLoop)(r14)
    lfs    f2, (menuAnimSpeed - mainLoop)(r14)
    fadds  f1, f1, f2
    lfs    f3, (one - mainLoop)(r14)
    fcmpo  0, f1, f3
    blt    .animContinue
    fmr    f1, f3 # clamp anim timer to 1

.animContinue:
    lwz    r19, (boxAddr - mainLoop)(r14)
    stfs   f1,  (menuAnimTimer - mainLoop)(r14) # f1 = timer
    bl     doAnimation

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
    addi  r18, r14, strBuf - mainLoop

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
    beq   .end
    add   r3, r3, r14
    mtspr CTR, r3
    mr    r3, r18
    bctrl
    # the item draw function should return r4=string,
    # and any additional sprintf args.
    mr    r3, r18 # strBuf
    CALL sprintf
    mr    r3, r18 # strBuf
    li    r4, 0x93 # box type
    #li    r5, MENU_XPOS + 8 # X pos
    lhz   r5, 0x0C(r19) # X pos
    addi  r5, r5, 10
    mr    r6, r16 # Y pos
    addi  r16, r16, LINE_HEIGHT
    CALL  gameTextShowStr
    b     .nextItem


.end:
    lwz  r5, SP_LR_SAVE(r1)
    mtlr r5 # restore LR
    lmw  r3, SP_GPR_SAVE(r1)
    addi r1, r1, STACK_SIZE # restore stack ptr
    blr

.closeAnim: # do close animation
    lwz    r3, (menuAnimTimer - mainLoop)(r14)
    cmpwi  r3, 0
    beq    .end # menu fully closed.

    lis    r4, 0x3F80 # 1.0 = 3F800000
    cmpw   r3, r4
    bne    .noCloseSound
    li     r3, 0
    li     r4, 0x03E6
    CALL   audioPlaySound
.noCloseSound:
    lfs    f1, (menuAnimTimer - mainLoop)(r14)
    lfs    f2, (menuAnimSpeed - mainLoop)(r14)
    fsubs  f1, f1, f2
    lfs    f3, (zero - mainLoop)(r14)
    fcmpo  0, f1, f3
    bgt    .closeAnimContinue
    fmr    f1, f3 # clamp anim timer to 0

.closeAnimContinue:
    lwz    r19, (boxAddr - mainLoop)(r14)
    stfs   f1,  (menuAnimTimer - mainLoop)(r14) # f1 = timer
    bl     doAnimation
    b      .drawMain

.close: # close the menu
    li     r3, 0
    stb    r3, (menuVisible - mainLoop)(r14)
    LOADWH r5, playerLocked
    STOREB r3, playerLocked, r5 # unpause game
    LOADWH r5, pauseDisabled
    STOREB r3, pauseDisabled, r5 # allow pause menu
    li     r4, 0x03F2
    CALL   audioPlaySound
    b      .end

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
    b      .end

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


doAnimation:
    # expects r19 = boxAddr, f1 = menuAnimTimer
    lfs    f2,  (f_menuWidth   - mainLoop)(r14) # f2 = width
    lfs    f3,  (f_menuHeight  - mainLoop)(r14) # f3 = height
    lfs    f4,  (f_centerX     - mainLoop)(r14) # f4 = X
    lfs    f5,  (f_centerY     - mainLoop)(r14) # f5 = Y
    lfs    f9,  (two           - mainLoop)(r14) # r9 = 2
    lfs    f8,  (twoFiveFive   - mainLoop)(r14) # r8 = 255
    fmuls  f8, f8, f1 # f8 = scaled alpha
    fctiwz f0, f8
    stfd   f0, SP_FLOAT_TMP(r1)
    lwz    r20, (SP_FLOAT_TMP+4)(r1)

    fmuls  f2, f2, f1 # f2 = scaled width
    fctiwz f0, f2
    stfd   f0, SP_FLOAT_TMP(r1)
    lwz    r6, (SP_FLOAT_TMP+4)(r1)
    sth    r6, 0x00(r19) # set box width

    fdivs  f2, f2, f9 # f2 /= 2
    fsubs  f6, f4, f2 # f6 = scaled X pos
    fctiwz f0, f6
    stfd   f0, SP_FLOAT_TMP(r1)
    lwz    r6, (SP_FLOAT_TMP+4)(r1)
    sth    r6, 0x0C(r19) # set box X pos

    fmuls  f3, f3, f1 # f3 = scaled height
    fctiwz f0, f3
    stfd   f0, SP_FLOAT_TMP(r1)
    lwz    r6, (SP_FLOAT_TMP+4)(r1)
    sth    r6, 0x02(r19) # set box height

    fdivs  f3, f3, f9 # f2 /= 2
    fsubs  f6, f5, f3 # f6 = scaled Y pos
    fctiwz f0, f6
    stfd   f0, SP_FLOAT_TMP(r1)
    lwz    r6, (SP_FLOAT_TMP+4)(r1)
    sth    r6, 0x0E(r19) # set box Y pos
    blr
