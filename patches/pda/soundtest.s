drawItem_sound:
    addi r4, r14, (s_sound - mainLoop)
    lhz  r5, (soundTestId - mainLoop)(r14)
    blr

adjItem_sound:
    # r3 = amount to adjust by (0 = A button)
    # r9 = sound effect to play after calling this
    lhz   r4, (soundTestId - mainLoop)(r14)
    add   r4, r4, r3
    sth   r4, (soundTestId - mainLoop)(r14)
    cmpwi r3, 0
    bnelr
    #mflr  r20
    # r3 is also the ObjInstance* which is null here
    #CALL  audioPlaySound
    #mtlr  r20
    lhz   r9, (soundTestId - mainLoop)(r14)
    blr
