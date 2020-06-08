drawItem_volMusic:
    addi    r4, r14, (s_volMusic - mainLoop)
    LOADWH  r5, volumeMusic
    LOADFL2 f1, volumeMusic, r5 # f1 = volume
drawItem_volume:
    lfs     f4, (oneHundred - mainLoop)(r14) # f4 = 100
    fmuls   f1, f1, f4 # f1 = vol%
    fctiw   f1, f1
    stfd    f1, SP_FLOAT_TMP(r1)
    lwz     r5, (SP_FLOAT_TMP+4)(r1)
    blr

adjItem_volMusic: # r3 = amount to adjust by
    LOAD    r5, volumeMusic

adjItem_volume:
    lfs     f2, (volAdjStep - mainLoop)(r14) # f2 = 0.1
    lfs     f3, (zero - mainLoop)(r14) # f3 = 0
    lfs     f4, (two - mainLoop)(r14) # f4 = 2
    b       adjItem_float

####################################################################

drawItem_volSFX:
    addi    r4, r14, (s_volSFX - mainLoop)
    LOADWH  r5, volumeSFX
    LOADFL2 f1, volumeSFX, r5 # f1 = volume
    b       drawItem_volume

adjItem_volSFX: # r3 = amount to adjust by
    LOAD    r5, volumeSFX
    b       adjItem_volume
