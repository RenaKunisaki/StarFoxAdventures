drawItem_bigMap: # r3 = strBuf
    addi  r4, r14, (s_MapSize - mainLoop)
    addi  r5, r14, (s_Normal - mainLoop)
    lhz   r6, (minimapSizeOverride - mainLoop)(r14)
    cmpwi r6, 0
    beq   .drawBigMap_off
    addi  r5, r14, (s_Huge - mainLoop)

.drawBigMap_off:
    blr

adjItem_bigMap: # r3 = amount to adjust by
    lhz     r3, (minimapSizeOverride - mainLoop)(r14)
    xori    r3, r3, 0x100
    sth     r3, (minimapSizeOverride - mainLoop)(r14)
    blr
