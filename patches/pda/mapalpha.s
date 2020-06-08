drawItem_mapAlpha: # r3 = strBuf
    addi r4, r14, (s_MapAlpha - mainLoop)
    lbz  r5, (minimapAlphaOverride - mainLoop)(r14)
    blr

adjItem_mapAlpha: # r3 = amount to adjust by
    mulli   r3, r3, 16
    lbz     r4, (minimapAlphaOverride - mainLoop)(r14)
    add     r4, r4, r3
    cmpwi   r4, 255
    ble     .adjMapAlpha_notMax
    li      r4, 255
.adjMapAlpha_notMax:
    cmpwi   r4, 0
    bge     .adjMapAlpha_notMin
    li      r4, 0
.adjMapAlpha_notMin:
    stb     r4, (minimapAlphaOverride - mainLoop)(r14)
    blr
