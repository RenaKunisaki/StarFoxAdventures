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

    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r5
    stw     r5,  SP_LR_SAVE(r1)
    stmw    r3,  SP_GPR_SAVE(r1)

    mflr    r3
    bl      .textHook_getpc
    .textHook_getpc: mflr r21
    mtlr    r3

    lfs     f2,  -0x6840(r13) #textScale - replaced

    LOADW   r3,  PATCH_STATE_PTR
    lbz     r3,  FORCE_TEXT_WIDTH(r3)
    cmpwi   r3,  0
    beq     .textHook_noOverride

    # convert r3 to float f0
    lfd     f1,  (.textHook_floatMagic - .textHook_getpc)(r21)
    lis     r0,  0x4330
    stw     r0,  SP_FLOAT_TMP(r1)
    xoris   r3,  r3,  0x8000
    stw     r3,  SP_FLOAT_TMP+4(r1)
    lfd     f0,  SP_FLOAT_TMP(r1)
    fsub    f0,  f0,  f1
    frsp    f0,  f0

    fmadds  f22, f2,  f0,  f22

    lwz     r0, SP_LR_SAVE(r1)
    mtlr    r0  # restore LR
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    JUMP    0x80018454, r21

.textHook_noOverride:
    lwz     r0, SP_LR_SAVE(r1)
    mtlr    r0  # restore LR
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    JUMP    0x80018418, r0

.textHook_floatMagic:    .int 0x43300000,0x80000000

textDrawHook:
    # when in fixed width mode, move some thin characters over
    # so they don't overlap the previous.
    LOADW   r9,  PATCH_STATE_PTR
    lbz     r9,  FORCE_TEXT_WIDTH(r9)
    cmpwi   r9,  0
    beq     .textDrawHook_end

    sub     r0,  r5,  r3
    cmpwi   r0,  0x28
    bge     .textDrawHook_end
    addi    r3,  r3,  8
    addi    r5,  r5,  8

.textDrawHook_end:
    JUMP    0x80075e8c, r0
