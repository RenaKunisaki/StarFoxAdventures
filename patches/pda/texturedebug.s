.ascii "txtrdbg " # 8 byte file ID for debug

.set TEXTURE_MENU_XPOS,    15
.set TEXTURE_MENU_YPOS,    40
.set TEXTURE_MENU_WIDTH,  620
.set TEXTURE_MENU_HEIGHT,  40
.set TEXTURE_IMAGE_XPOS,   15
.set TEXTURE_IMAGE_YPOS,   85
.set TEXTURE_IMAGE_PADDING, 7

textureImageXposFloat: .float  22
textureImageYposFloat: .float  92

textureMenu:
    # subroutine: runs the Texture Debug menu.
    # expects r14 = mainLoop.
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    bl      menuHideHud
    bl      menuSetFixedWidth
    bl      textureMenu_doInput
    bl      textureMenu_Main
    b       menuEndSub

textureMenu_Main: # draw texture info
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    # draw the boxes
    li      r3,  TEXTURE_MENU_XPOS   # X
    li      r4,  TEXTURE_MENU_YPOS   # Y
    li      r5,  TEXTURE_MENU_WIDTH  # width
    li      r6,  TEXTURE_MENU_HEIGHT # height
    bl      menuDrawBoxOpaque

    LOAD    r3,  0xFFFFFFFF
    bl      menuSetTextColor

    LOADW   r15, 0x803dcdc4 # loaded textures

    # draw info
    li      r19, TEXTURE_MENU_XPOS + 8 # line X pos
    li      r20, TEXTURE_MENU_YPOS + 8 # line Y pos
    addi    r4,  r14, s_textureId - mainLoop
    lhz     r5,  (textureDebugId - mainLoop)(r14)
    lbz     r6,  (textureDebugMip - mainLoop)(r14)
    li      r7,  ' '
    slwi    r8,  r5,  4
    add     r16, r8,  r15 # LoadedTexture*
    li      r8,  0
    lwz     r9,  0x04(r16) # Texture*
    cmpwi   r9,  0
    beq     .textureMenu_main_drawInfo_noTexture
    lwz     r8,  0x00(r16) # ID
    cmpwi   r8,  0
    bge     .textureMenu_main_drawInfo_posId
    neg     r8,  r8
    li      r7,  '-'

.textureMenu_main_drawInfo_posId:
    bl      menuPrintf

    # draw more info
    addi    r20, r20, LINE_HEIGHT
    addi    r4,  r14, s_textureInfo - mainLoop
    lwz     r3,  0x04(r16) # Texture*
    lwz     r5,  0x04(r3)  # flags
    lhz     r6,  0x0E(r3)  # ref count
    lbz     r7,  0x16(r3)  # format
    lhz     r8,  0x0A(r3)  # width
    lhz     r9,  0x0C(r3)  # height
.textureMenu_main_drawInfo_noTexture:
    bl      menuPrintf


    # draw texture
    lhz     r3,  (textureDebugId - mainLoop)(r14)
    slwi    r3,  r3,  4
    add     r3,  r3,  r15 # LoadedTexture*
    lwz     r16, 0x04(r3) # Texture*
    lbz     r4,  (textureDebugMip - mainLoop)(r14)

.textureMenu_main_draw_nextMip:
    cmpwi   r16, 0
    beq     menuEndSub # no texture to draw
    cmpwi   r4,  0
    beq     .textureMenu_main_draw_gotMip
    lwz     r16, 0(r16)
    subi    r4,  r4,  1
    b       .textureMenu_main_draw_nextMip

.textureMenu_main_draw_gotMip:
    lhz     r5,  0x0A(r16) # width
    lhz     r6,  0x0C(r16) # height
    li      r3,  TEXTURE_IMAGE_XPOS   # X
    li      r4,  TEXTURE_IMAGE_YPOS   # Y
    addi    r5,  r5,  TEXTURE_IMAGE_PADDING # width
    addi    r6,  r6,  TEXTURE_IMAGE_PADDING # height
    # XXX shouldn't this be TEXTURE_IMAGE_PADDING * 2? but that's too much.
    # it shouldn't be too much, so what's going on there?
    bl      menuDrawBoxOpaque

    mr      r3,  r16
    li      r4,  0xFF # opacity
    li      r5,  0x0100 # scale
    lfs     f1,  (textureImageXposFloat - mainLoop)(r14)
    lfs     f2,  (textureImageYposFloat - mainLoop)(r14)
    CALL    0x8007719c # draw texture
    b       menuEndSub

    # seems like each texture has at offset 0 a pointer to the next mipmap...


textureMenu_doInput:
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    # check the buttons
    bl      menuGetInput
    # r3=buttons, r4=stick X, r5=stick Y,
    # r6=CX, r7=CY, r8=L, r9=R
    andi.   r10, r3,  PAD_BUTTON_B
    bne     .textureMenu_close

    lhz     r17, (textureDebugId - mainLoop)(r14)
    li      r18, 0 # menu delay

    # check D pad
    andi.   r0,  r3,  PAD_BUTTON_LEFT
    bne     .textureMenu_prevMip
    andi.   r0,  r3,  PAD_BUTTON_RIGHT
    bne     .textureMenu_nextMip

    # check L/R
    cmpwi   r8,  4
    bgt     .textureMenu_downFast
    cmpwi   r9,  4
    bgt     .textureMenu_upFast

    # check analog stick
    cmpwi   r4,  -0x10
    blt     .textureMenu_down
    cmpwi   r4,   0x10
    ble     menuEndSub

.textureMenu_up: # right pressed
    li      r18, MOVE_DELAY
.textureMenu_upFast:
    addi    r17, r17, 1
.textureMenu_store:
    sth     r17, (textureDebugId - mainLoop)(r14)
.textureMenu_storeDelay:
    stb     r18, (menuJustMoved  - mainLoop)(r14)
    b       menuEndSub

.textureMenu_down: # left pressed
    li      r18, MOVE_DELAY
.textureMenu_downFast:
    subi    r17, r17, 1
    b       .textureMenu_store

.textureMenu_prevMip:
    li      r4,  -1
.textureMenu_storeMip:
    lbz     r3,  (textureDebugMip - mainLoop)(r14)
    add     r3,  r3,  r4
    stb     r3,  (textureDebugMip - mainLoop)(r14)
    li      r18, MOVE_DELAY
    b       .textureMenu_storeDelay

.textureMenu_nextMip:
    li      r4,  1
    b       .textureMenu_storeMip


.textureMenu_close:
    bl      returnToMainMenu
    b       menuEndSub
