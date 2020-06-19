menuDrawBoxOpaque:
    li r20, 0xFF

menuDrawBox:
    # subroutine: draws a box.
    # expects r14 = mainLoop.
    # r3=X, r4=Y, r5=W, r6=H, r20=opacity (from doAnimation)
    # This is pretty much copied from gameTextDrawBox().
    # Unfortunately I can't find a function that just draws a box
    # without any hardcoded parameters.
    .set   BOX_BORDER_WIDTH,5 # width of border texture
    # offsets into hudTextures
    .set   BOX_TEXTURE_CORNER,  (10 * 4)
    .set   BOX_TEXTURE_LEFT,    (11 * 4)
    .set   BOX_TEXTURE_INTERIOR,(12 * 4)
    .set   BOX_TEXTURE_TOP,     (13 * 4)
    .set   BOX_FLAG_FLIP_H, 1
    .set   BOX_FLAG_FLIP_V, 2

    stwu   r1, -STACK_SIZE(r1) # get some stack space
    mflr   r9
    stw    r9, SP_LR_SAVE(r1)
    stmw   r13, SP_GPR_SAVE(r1)

    LOAD   r15, 0x803a89b0 # HUD textures
    mr     r16, r3 # X
    mr     r17, r4 # Y
    mr     r18, r5 # W
    mr     r19, r6 # H
    mflr   r29

    # save these for reference when positioning text.
    sth    r3, (menuBoxX - mainLoop)(r14)
    sth    r4, (menuBoxY - mainLoop)(r14)
    sth    r5, (menuBoxW - mainLoop)(r14)
    sth    r6, (menuBoxH - mainLoop)(r14)

    # convert X, Y to float
    bl      intToFloat # X
    fmr     f3, f1
    mr      r3, r4
    bl      intToFloat # Y
    fmr     f2, f1
    fmr     f1, f3

    # draw top left corner
    lwz    r3, BOX_TEXTURE_CORNER(r15) # upper left corner texture
    #li     r4, 0xFF # opacity
    mr     r4, r20 # opacity set by doAnimation
    li     r5, 0x0100 # scale
    CALL   0x8007719c # draw texture

    # draw top
    mr     r3, r17
    bl     intToFloat # Y
    fmr    f2, f1
    addi   r3, r16, BOX_BORDER_WIDTH
    bl     intToFloat # X
    #li     r4, 0xFF # opacity
    mr     r4, r20 # opacity set by doAnimation
    li     r5, 0x0100 # scale
    mr     r6, r18 # width
    li     r7, BOX_BORDER_WIDTH # height
    li     r8, 0 # flags
    lwz    r3, BOX_TEXTURE_TOP(r15) # texture
    CALL   0x8007681c # draw scaled texture

    # draw left
    addi   r3, r17, BOX_BORDER_WIDTH
    bl     intToFloat # Y
    fmr    f2, f1
    mr     r3, r16
    bl     intToFloat # X
    #li     r4, 0xFF # opacity
    mr     r4, r20 # opacity set by doAnimation
    li     r5, 0x0100 # scale
    li     r6, BOX_BORDER_WIDTH # width
    mr     r7, r19 # height
    li     r8, 0 # flags
    lwz    r3, BOX_TEXTURE_LEFT(r15) # texture
    CALL   0x8007681c # draw scaled texture

    # draw interior
    addi   r3, r17, BOX_BORDER_WIDTH
    bl     intToFloat # Y
    fmr    f2, f1
    addi   r3, r16, BOX_BORDER_WIDTH
    bl     intToFloat # X
    #li     r4, 0xFF # opacity
    mr     r4, r20 # opacity set by doAnimation
    li     r5, 0x0100 # scale
    mr     r6, r18 # width
    mr     r7, r19 # height
    li     r8, 4 # flags - changes blending to make text look nicer.
    # unsure exactly what this flag does.
    lwz    r3, BOX_TEXTURE_INTERIOR(r15) # texture
    CALL   0x8007681c # draw scaled texture

    # draw bottom
    add    r3, r17, r19
    addi   r3, r3, BOX_BORDER_WIDTH
    bl     intToFloat # Y
    fmr    f2, f1
    addi   r3, r16, BOX_BORDER_WIDTH
    bl     intToFloat # X
    #li     r4, 0xFF # opacity
    mr     r4, r20 # opacity set by doAnimation
    li     r5, 0x0100 # scale
    mr     r6, r18 # width
    li     r7, BOX_BORDER_WIDTH # height
    li     r8, BOX_FLAG_FLIP_V # flags
    lwz    r3, BOX_TEXTURE_TOP(r15) # texture
    CALL   0x8007681c # draw scaled texture

    # draw right
    addi   r3, r17, BOX_BORDER_WIDTH
    bl     intToFloat # Y
    fmr    f2, f1
    add    r3, r16, r18
    addi   r3, r3, BOX_BORDER_WIDTH
    bl     intToFloat # X
    #li     r4, 0xFF # opacity
    mr     r4, r20 # opacity set by doAnimation
    li     r5, 0x0100 # scale
    li     r6, BOX_BORDER_WIDTH # width
    mr     r7, r19 # height
    li     r8, BOX_FLAG_FLIP_H # flags
    lwz    r3, BOX_TEXTURE_LEFT(r15) # texture
    CALL   0x8007681c # draw scaled texture

    # draw top right corner
    mr     r3, r17
    bl     intToFloat # Y
    fmr    f2, f1
    add    r3, r16, r18
    addi   r3, r3, BOX_BORDER_WIDTH
    bl     intToFloat # X
    #li     r4, 0xFF # opacity
    mr     r4, r20 # opacity set by doAnimation
    li     r5, 0x0100 # scale
    li     r6, BOX_BORDER_WIDTH # width
    li     r7, BOX_BORDER_WIDTH # height
    li     r8, BOX_FLAG_FLIP_H # flags
    lwz    r3, BOX_TEXTURE_CORNER(r15) # texture
    CALL   0x8007681c # draw scaled texture

    # draw bottom left corner
    add    r3, r17, r19
    addi   r3, r3, BOX_BORDER_WIDTH
    bl     intToFloat # Y
    fmr    f2, f1
    mr     r3, r16
    bl     intToFloat # X
    #li     r4, 0xFF # opacity
    mr     r4, r20 # opacity set by doAnimation
    li     r5, 0x0100 # scale
    li     r6, BOX_BORDER_WIDTH # width
    li     r7, BOX_BORDER_WIDTH # height
    li     r8, BOX_FLAG_FLIP_V # flags
    lwz    r3, BOX_TEXTURE_CORNER(r15) # texture
    CALL   0x8007681c # draw scaled texture

    # draw bottom right corner
    add    r3, r17, r19
    addi   r3, r3, BOX_BORDER_WIDTH
    bl     intToFloat # Y
    fmr    f2, f1
    add    r3, r16, r18
    addi   r3, r3, BOX_BORDER_WIDTH
    bl     intToFloat # X
    #li     r4, 0xFF # opacity
    mr     r4, r20 # opacity set by doAnimation
    li     r5, 0x0100 # scale
    li     r6, BOX_BORDER_WIDTH # width
    li     r7, BOX_BORDER_WIDTH # height
    li     r8, BOX_FLAG_FLIP_V | BOX_FLAG_FLIP_H # flags
    lwz    r3, BOX_TEXTURE_CORNER(r15) # texture
    CALL   0x8007681c # draw scaled texture

    mtlr   r29
    b      menuEndSub

menuBoxX: .short 0
menuBoxY: .short 0
menuBoxW: .short 0
menuBoxH: .short 0
