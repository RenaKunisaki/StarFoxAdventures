# PDA patch:
# replaces the "PDA On/Off" item with a menu.
.text
.include "common.s"

# define patches
patchList:
    PATCH_ID "PDAMenu" # must be 7 chars
    PATCH_BL 0x80133A54, itemHook
    PATCH_MAIN_LOOP mainLoop
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set MENU_WIDTH,400
    .set MENU_HEIGHT,200
    .set MENU_XPOS,320-(MENU_WIDTH/2)
    .set MENU_YPOS,240-(MENU_HEIGHT/2)
    .set LINE_HEIGHT,18
    .set STACK_SIZE,0x100 # how much to reserve
    .set SP_LR_SAVE,0x104 # this is what the game does
    .set SP_FLOAT_TMP,0x20 # temporary storage for float conversion (8 bytes)
    .set SP_GPR_SAVE,0x28

entry: # called as soon as our patch is loaded.
    # nothing to do
    blr


itemHook: # called when the PDA is being toggled on/off by player.
    # r13 = &pdaOn + 0x7630
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r5
    stw   r5, SP_LR_SAVE(r1)
    stmw  r3, SP_GPR_SAVE(r1)

    bl .itemHook_getpc
    .itemHook_getpc:
        mflr r29

    li   r3, 1
    stb  r3, (menuVisible - .itemHook_getpc)(r29)

    lwz  r5, SP_LR_SAVE(r1)
    mtlr r5 # restore LR
    lmw  r3, SP_GPR_SAVE(r1)
    addi r1, r1, STACK_SIZE # restore stack ptr

    JUMP 0x80133A94, r0 # skip the actual on/off code


mainLoop: # called from main loop. r3 = mainLoop
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r5
    stw   r5, SP_LR_SAVE(r1)
    stmw  r3, SP_GPR_SAVE(r1)
    mr    r14, r3

    lbz   r4, (menuVisible - mainLoop)(r14)
    cmpwi r4, 0
    beq   .closeAnim

    # ------ menu is visible ------
    # pause game
    LOADWH r5, playerLocked
    STOREB r4, playerLocked, r5

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
    andi.   r8, r8, 0xF0 # deadzone

    lbz     r5, (menuJustMoved - mainLoop)(r14)
    cmpwi   r5, 0
    beq     .checkMove
    # we already moved. don't move again until joystick is reset.
    cmpwi   r8, 0
    bne     .draw
    b       .resetJustMoved # r7 must be 0


.checkMove: # Check if joystick moved.
    cmpwi   r7, 0x18
    bgt     .up
    cmpwi   r7, -0x18
    blt     .down
    cmpwi   r6, 0x18
    bgt     .right
    cmpwi   r6, -0x18
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
    #mr    r3, r18 # strBuf
    li    r4, 0x93 # box type
    #li    r5, MENU_XPOS + 8 # X pos
    lhz   r5, 0x0C(r19) # X pos
    mr    r6, r16 # Y pos
    addi  r16, r16, LINE_HEIGHT
    CALL  gameTextShowStr
    b     .nextItem
    # 81682b64
    # 802c7593


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
    li     r4, 0x03E6
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


drawItem_player:
    # r3 = strBuf
    # XXX how to get the variable?
    addi r4, r14, (s_Player - mainLoop)
    addi r5, r14, (s_Krystal - mainLoop)
    mflr r28
    CALL sprintf
    mtlr r28
    mr   r3, r18 # return strbuf
    blr

adjItem_player:
    # r3 = amount to adjust by
    blr

drawItem_PDAHUD:
    addi    r4, r14, (s_PDAHUD - mainLoop)
    LOADWH  r7, pdaOn
    LOADBL2 r6, pdaOn, r7
    cmpwi   r6, 0
    beq     .draw_pda_off
    LOADBL2 r6, minimapMode, r7
    b       .draw_pda

.draw_pda_off:
    li      r6, 3

.draw_pda:
    slwi    r6, r6, 1 # r6 = offs into pdaModeStrs
    addi    r6, r6, .pdaModeStrs - mainLoop # r6 = addr of str offs
    lhzx    r6, r6, r14 # r6 = str offs from mainLoop
    add     r5, r6, r14 # r5 = str
    mflr    r28
    CALL    sprintf
    mtlr    r28
    mr      r3, r18 # return strbuf
    blr

.pdaModeStrs:
    .short s_map     - mainLoop
    .short s_compass - mainLoop
    .short s_info    - mainLoop
    .short s_off     - mainLoop

adjItem_PDAHUD:
    LOADWH  r7, pdaOn
    LOADBL2 r6, minimapMode, r7 # shares same upper half addr
    LOADBL2 r8, pdaOn, r7
    cmpwi   r8, 0
    bne     .adjPDA
    li      r6, 3

.adjPDA:
    add     r6, r6, r3 # r3 = adjust amount
    andi.   r6, r6, 3
    cmpwi   r6, 3
    beq     .pdaOff
    STOREB  r6, minimapMode, r7
    li      r6, 1
    b       .setPDA
.pdaOff:
    li      r6, 0

.setPDA:
    STOREB  r6, pdaOn, r7
    blr


# unfortunately this isn't safe.
# playing invalid sounds can crash the game.
drawItem_sound:
    # r3 = strBuf
    addi r4, r14, (s_sound - mainLoop)
    lhz  r5, (soundTestId - mainLoop)(r14)
    mflr r28
    CALL sprintf
    mtlr r28
    mr   r3, r18 # return strbuf
    blr

adjItem_sound:
    # r3 = amount to adjust by
    lhz  r4, (soundTestId - mainLoop)(r14)
    add  r4, r4, r3
    sth  r4, (soundTestId - mainLoop)(r14)
    li   r3, 0
    CALL audioPlaySound
    blr


itemDrawFuncs:
    .int drawItem_player - mainLoop
    .int drawItem_PDAHUD - mainLoop
    #.int drawItem_sound  - mainLoop
    .int 0

itemAdjustFuncs:
    .int adjItem_player - mainLoop
    .int adjItem_PDAHUD - mainLoop
    #.int adjItem_sound  - mainLoop

# more items to add:
# - FOV (will require overriding some code when not default)
# - sound volumes
# - debug print
# - time scale

# address of GameTextBox we use
boxAddr: .int 0x802c7588

# menu animation
menuAnimTimer: .float 0
menuAnimSpeed: .float 0.125 # 1 / 8
#menuAnimSpeed: .float 0.066666667 # 1 / 15
#menuAnimSpeed: .float 0.0166666667 # 1 / 60

# float constants
zero:          .float 0
one:           .float 1
two:           .float 2
twoFiveFive:   .float 255
floatMagic:    .int 0x43300000,0x80000000
# lol of course we can't do this.
#f_menuWidth:   .float MENU_WIDTH
#f_menuHeight:  .float MENU_HEIGHT
#f_menuXPos:    .float MENU_XPOS
#f_menuYPos:    .float MENU_YPOS
f_menuWidth:   .float 400
f_menuHeight:  .float 200
f_centerX:     .float 320
f_centerY:     .float 240

# menu state
menuVisible:   .byte 0
menuSelItem:   .byte 0
menuSelColor:  .byte 0
menuJustMoved: .byte 0
soundTestId:   .short 0

# string pool
title:     .string "PDA Menu"
s_on:      .string "On"
s_off:     .string "Off"
s_Player:  .string "Player: %s"
s_Fox:     .string "Fox"
s_Krystal: .string "Krystal"
s_PDAHUD:  .string "PDA HUD: %s"
s_map:     .string "Map"
s_compass: .string "Fuel Cell Compass"
s_info:    .string "Information"
s_sound:   .string "Sound Test: %04X"

strBuf: # buffer to print strings into
    .rept 64
    .byte 0
    .endr
