.set TEXTURE_MENU_XPOS,    5
.set TEXTURE_MENU_YPOS,   40
.set TEXTURE_MENU_WIDTH, 310
.set TEXTURE_MENU_HEIGHT,400
.set TEXTURE_INFO_XPOS,  320
.set TEXTURE_INFO_YPOS,   40
.set TEXTURE_INFO_WIDTH, 310
.set TEXTURE_INFO_HEIGHT,400

textureInfoXposFloat: .float 328
textureInfoYposFloat: .float 48

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

    li      r3,  TEXTURE_INFO_XPOS   # X
    li      r4,  TEXTURE_INFO_YPOS   # Y
    li      r5,  TEXTURE_INFO_WIDTH  # width
    li      r6,  TEXTURE_INFO_HEIGHT # height
    bl      menuDrawBoxOpaque

    LOAD    r3,  0xFFFFFFFF
    bl      menuSetTextColor

    LOADW   r15, 0x803dcdc4 # loaded textures

    # draw info
    li      r19, TEXTURE_MENU_XPOS + 8 # line X pos
    li      r20, TEXTURE_MENU_YPOS + 8 # line Y pos
    addi    r4,  r14,  s_textureId - mainLoop
    lhz     r5,  (textureDebugId - mainLoop)(r14)
    slwi    r6,  r5,  4
    add     r9,  r6,  r15 # LoadedTexture*
    li      r6,  0
    li      r7,  0
    cmpwi   r9,  0
    beq     .textureMenu_main_drawInfo_noTexture
    lwz     r6,  0x00(r9) # ID
    lwz     r7,  0x04(r9) # Texture*
.textureMenu_main_drawInfo_noTexture:
    bl      menuPrintf

    # draw texture
    lhz     r3,  (textureDebugId - mainLoop)(r14)
    slwi    r3,  r3,  4
    add     r3,  r3,  r15 # LoadedTexture*
    lwz     r3,  0x04(r3) # Texture*
    cmpwi   r3,  0
    beq     menuEndSub # no texture to draw

    li      r4,  0xFF # opacity
    li      r5,  0x0100 # scale
    lfs     f1,  (textureInfoXposFloat - mainLoop)(r14)
    lfs     f2,  (textureInfoYposFloat - mainLoop)(r14)
    CALL    0x8007719c # draw texture
    b       menuEndSub

    # we should draw the second box around the texture itself, at its actual size,
    # and have two boxes vertically instead of horizontally.
    # also, find some way to view mipmaps.


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
    stb     r18, (menuJustMoved  - mainLoop)(r14)
    b       menuEndSub

.textureMenu_down: # left pressed
    li      r18, MOVE_DELAY
.textureMenu_downFast:
    subi    r17, r17, 1
    b       .textureMenu_store

.textureMenu_close:
    bl      returnToMainMenu
    b       menuEndSub
