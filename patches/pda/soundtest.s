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

######################################################################

drawItem_music:
    addi  r4, r14, (s_music - mainLoop)
    lhz   r5, (musicTestId - mainLoop)(r14)
    LOAD  r6, 0x802c5700 # get song entry
    slwi  r7, r5, 4 # r7 = r5 * 16
    add   r6, r6, r7
    lwz   r6, 4(r6) # get name ptr
    blr

adjItem_music:
    # r3 = amount to adjust by (0 = A button)
    # r9 = sound effect to play after calling this
    lhz   r4, (musicTestId - mainLoop)(r14)
    add   r4, r4, r3
    cmpwi r4, 0
    bge   r4, .musicNotMax
    li    r4, 0x58
.musicNotMax:
    cmpwi r4, 0x59
    blt   .musicNotMin
    li    r4, 0
.musicNotMin:
    sth   r4, (musicTestId - mainLoop)(r14)
    cmpwi r3, 0
    bnelr
    mflr  r20
    lhz   r3, (musicTestId - mainLoop)(r14)
    CALL  0x8000a2e4 # music test function
    mtlr  r20
    li    r9, 0 # no sound effect
    blr
