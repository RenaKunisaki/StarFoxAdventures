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

####################################################################

drawItem_volCutScene:
    # this one is inexplicably not a float.
    # it ranges from 0 to 127.
    # XXX does going beyond 127 actually do something?
    addi    r4, r14, (s_volCS - mainLoop)
    LOADB   r5, volumeCutScenes
    mulli   r5, r5, 1000
    li      r6, 12700 # PPC Y U NO DIVLI?
    divw    r5, r5, r6
    mulli   r5, r5, 10 # quick hack to hide rounding error.
    blr

adjItem_volCutScene: # r3 = amount to adjust by
    mulli   r3, r3, 13
    LOADWH  r5, volumeCutScenes
    LOADBL2 r4, volumeCutScenes, r5
    add     r4, r4, r3
    cmpwi   r4, 255
    ble     .adjVolCs_notMax
    li      r4, 255
.adjVolCs_notMax:
    cmpwi   r4, 0
    bge     .adjVolCs_notMin
    li      r4, 0
.adjVolCs_notMin:
    STOREB  r4, volumeCutScenes, r5
    blr
