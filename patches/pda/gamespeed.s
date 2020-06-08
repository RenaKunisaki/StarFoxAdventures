drawItem_gameSpeed:
    addi    r4, r14, (s_gameSpeed - mainLoop)
    LOADWH  r5, physicsTimeScale
    LOADFL2 f1, physicsTimeScale, r5 # f1 = speed
    lfs     f4, (gameSpeedDiv - mainLoop)(r14) # f4 = 60/100
    fdivs   f1, f1, f4 # f1 = speed%
    fctiw   f1, f1
    stfd    f1, SP_FLOAT_TMP(r1)
    lwz     r5, (SP_FLOAT_TMP+4)(r1)
    blr

adjItem_gameSpeed: # r3 = amount to adjust by
    LOAD    r5, physicsTimeScale
    lfs     f2, (gameSpeedMin - mainLoop)(r14) # f2 = 15
    fmr     f3, f2 # f3 = 15
    lfs     f4, (gameSpeedMax - mainLoop)(r14) # f4 = 240
    b       adjItem_float
