drawItem_FOV:
    mr      r7, r3
    LOADWH  r5, fovY
    LOADFL2 f1, fovY, r5
    fctiwz  f1, f1
    stfd    f1, SP_FLOAT_TMP(r1)
    lwz     r5, (SP_FLOAT_TMP+4)(r1)

    # if FOV is 60 (default), undo the override patch.
    # otherwise, apply it.
    # doing this here instead of on adjust to simplify
    # the code a bit.
    LOAD    r3, 0x8000FC4C # patch address
    lis     r4, 0x6000 # NOP
    cmpwi   r5, 60
    bne     .fov_doPatch
    lwz     r4, (fovOpDefault - mainLoop)(r14)
.fov_doPatch:
    stw     r4, 0(r3)
    li      r4, 0
    icbi    r4, r3 # flush instruction cache

    mr      r3, r7
    addi    r4, r14, (s_FOV - mainLoop)
    blr

adjItem_FOV: # r3 = amount to adjust by
    LOAD    r5, fovY
    lfs     f2, (five   - mainLoop)(r14) # f2 = step
    lfs     f3, (five   - mainLoop)(r14) # f3 = min
    lfs     f4, (fovMax - mainLoop)(r14) # f4 = max
    b       adjItem_float
