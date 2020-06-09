drawItem_FOV:
    mr      r7, r3
    lfs     f1, (fovOverride - mainLoop)(r14)
    fctiwz  f1, f1
    stfd    f1, SP_FLOAT_TMP(r1)
    lwz     r5, (SP_FLOAT_TMP+4)(r1)
    mr      r3, r7
    addi    r4, r14, (s_FOV - mainLoop)
    blr

adjItem_FOV: # r3 = amount to adjust by
    #LOAD    r5, fovY
    addi    r5, r14, fovOverride - mainLoop
    lfs     f2, (five   - mainLoop)(r14) # f2 = step
    lfs     f3, (five   - mainLoop)(r14) # f3 = min
    lfs     f4, (fovMax - mainLoop)(r14) # f4 = max
    b       adjItem_float

fovOverride: .float 60
