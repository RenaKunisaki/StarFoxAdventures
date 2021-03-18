.text
.include "common.s"
.global textHook
.global textDrawHook
.global textForceFixedWidth

textHook:
    # hook into textRenderStr and override the calculation
    # of the character width.
    # there IS a fixed-width mode, but only for debug text.
    # unfortunately the hooked code isn't run right when we render the string,
    # so we have to keep this override active as long as we're displaying
    # any text we want to be fixed width, not just when we render it.
    # that means it will affect all text on screen.
    # f22 = current X position
    # r22 = character

    stwu    r1,  -0x80(r1) # get some stack space
    mflr    r5
    stw     r5,   0x84(r1)
    stmw    r3,   0x20(r1)

    mflr    r3
    bl      .textHook_getpc
    .textHook_getpc: mflr r21
    mtlr    r3

    lfs     f2,  -0x6840(r13) # textScale - replaced

    lbz     r3,  (textForceFixedWidth - .textHook_getpc)(r21)
    cmpwi   r3,  0
    beq     .textHook_noOverride

    # convert r3 to float f0
    lfd     f1,  (.textHook_floatMagic - .textHook_getpc)(r21)
    lis     r0,  0x4330
    stw     r0,  0x18(r1)
    xoris   r3,  r3,  0x8000
    stw     r3,  0x18+4(r1)
    lfd     f0,  0x18(r1)
    fsub    f0,  f0,  f1
    frsp    f0,  f0

    fmadds  f22, f2,  f0,  f22

    lwz     r0,  0x84(r1)
    mtlr    r0   # restore LR
    lmw     r3,  0x20(r1)
    addi    r1,  r1,  0x80 # restore stack ptr
    JUMP    0x80018454, r21

.textHook_noOverride:
    lwz     r0,  0x84(r1)
    mtlr    r0   # restore LR
    lmw     r3,  0x20(r1)
    addi    r1,  r1,  0x80 # restore stack ptr
    JUMP    0x80018418, r0


textDrawHook:
    # when in fixed width mode, move some thin characters over
    # so they don't overlap the previous.
    mflr    r8
    bl      .textDrawHook_getpc
    .textDrawHook_getpc: mflr r9
    mtlr    r8

    lbz     r9,  (textForceFixedWidth - .textDrawHook_getpc)(r9)
    cmpwi   r9,  0
    beq     .textDrawHook_end

    sub     r0,  r5,  r3
    cmpwi   r0,  0x28
    bge     .textDrawHook_end
    addi    r3,  r3,  8
    addi    r5,  r5,  8

.textDrawHook_end:
    JUMP    0x80075e8c, r0


.textHook_floatMagic: .int 0x43300000,0x80000000
textForceFixedWidth:  .byte 0
