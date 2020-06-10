drawItem_sound:
    addi r4, r14, (s_sound - mainLoop)
    lhz  r5, (soundTestId - mainLoop)(r14)
    blr

adjItem_sound:
    # r3 = amount to adjust by (0 = A button)
    # r9 = sound effect to play after calling this
    lha   r4, (soundTestId - mainLoop)(r14)
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
    lbz   r5, (musicTestId - mainLoop)(r14)
    LOAD  r6, 0x802c5700 # get song entry
    slwi  r7, r5, 4 # r7 = r5 * 16
    add   r6, r6, r7
    lwz   r6, 4(r6) # get name ptr
    blr

adjItem_music:
    # r3 = amount to adjust by (0 = A button)
    # r9 = sound effect to play after calling this
    lbz   r4, (musicTestId - mainLoop)(r14)
    #extsb r4, r4
    add   r4, r4, r3
    cmpwi r4, 0
    bge   .musicNotMax
    li    r4, 0x58
.musicNotMax:
    cmpwi r4, 0x59
    blt   .musicNotMin
    li    r4, 0
.musicNotMin:
    stb   r4, (musicTestId - mainLoop)(r14)
    cmpwi r3, 0
    bnelr
    mflr  r20
    lbz   r3, (musicTestId - mainLoop)(r14)
    CALL  0x8000a2e4 # music test function
    mtlr  r20
    li    r9, 0 # no sound effect
    blr

######################################################################

drawItem_streamTest:
    addi  r4, r14, (s_stream - mainLoop)
    lhz   r5, (streamTestId - mainLoop)(r14)
    LOADW r6, 0x803dc850 # STREAMS.bin
    mulli r7, r5, 0x16
    add   r6, r6, r7
    addi  r6, r6, 6 # to name
    blr

adjItem_streamTest:
    # r3 = amount to adjust by (0 = A button)
    # r9 = sound effect to play after calling this
    lha   r4, (streamTestId - mainLoop)(r14)
    add   r4, r4, r3
    cmpwi r4, 0
    bge   .streamNotMax
    li    r4, 0x33D
.streamNotMax:
    cmpwi r4, 0x33E
    blt   .streamNotMin
    li    r4, 0
.streamNotMin:
    sth   r4, (streamTestId - mainLoop)(r14)
    cmpwi r3, 0
    bnelr

    mflr  r20
    #CALL  0x8000d594 # reset stream
    CALL  0x8000d01c # stop stream

    lhz   r3, (streamTestId - mainLoop)(r14)
    LOADW r6, 0x803dc850 # STREAMS.bin
    mulli r7, r3, 0x16
    add   r6, r6, r7
    lhz   r3, 0(r6) # get ID
    LOAD  r4, 0x8000d138 # callback the game uses
    CALL  0x8000d200 # play stream
    mtlr  r20
    li    r9, 0 # no sound effect
    blr
