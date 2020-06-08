drawItem_sound:
    # r3 = strBuf
    addi r4, r14, (s_sound - mainLoop)
    lhz  r5, (soundTestId - mainLoop)(r14)
    blr

adjItem_sound:
    # r3 = amount to adjust by
    lhz  r4, (soundTestId - mainLoop)(r14)
    add  r4, r4, r3
    sth  r4, (soundTestId - mainLoop)(r14)
    li   r3, 0
    CALL audioPlaySound
    blr
