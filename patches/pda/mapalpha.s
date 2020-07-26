drawItem_mapAlpha:
    addi  r4,  r14, (s_MapAlpha - mainLoop)
    lbz   r5,  (minimapAlphaOverride - mainLoop)(r14)
    # convert to percent
    mulli r5,  r5,  1000
    li    r6,  25500 # PPC Y U NO DIVLI?
    divw  r5,  r5,  r6
    mulli r5,  r5,  10 # quick hack to hide rounding error.
    blr

adjItem_mapAlpha: # r3 = amount to adjust by
    mulli   r3,  r3, 25
    lbz     r4,  (minimapAlphaOverride - mainLoop)(r14)
    add     r4,  r4, r3
    cmpwi   r4,  255
    ble     .adjMapAlpha_notMax
    li      r4,  255
.adjMapAlpha_notMax:
    cmpwi   r4,  0
    bge     .adjMapAlpha_notMin
    li      r4,  0
.adjMapAlpha_notMin:
    stb     r4,  (minimapAlphaOverride - mainLoop)(r14)
    LOADWH  r5,  saveData
    STOREB  r4,  SAVEDATA_MAP_OPACITY+saveData, r5
    blr
