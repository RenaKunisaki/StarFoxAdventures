# correct eye direction

eyePatch: # yarr
    # 80087050 EC 00 00 72  fmuls    f0,f0,f1
    # f0 = 10.0, f1 = eye offset
    fmuls   f0,  f0,  f1 # replaced
    LOADW   r4,  PATCH_STATE_PTR
    lbz     r4,  CUR_CHAR_ADDR(r4)
    andi.   r4,  r4,  0x7F
    bne     .eyePatch_normal
    mflr    r4
    bl      .eyePatch_getpc
    .eyePatch_getpc: mflr r3
    mtlr    r4
    lfs     f1,  (.eyePatch_mul - .eyePatch_getpc)(r3)
    fmuls   f0,  f0,  f1

.eyePatch_normal:
    JUMP    0x80087054,  r4

.eyePatch_mul:
    # Krystal's eyes don't have the same range as Fox's,
    # so make sure they don't look so far to the side that
    # they go inside her skull.
    .float -0.7
