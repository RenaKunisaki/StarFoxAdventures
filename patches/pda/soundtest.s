.ascii "sndtest " # 8 byte file ID for debug

drawItem_sound:
    addi    r4,  r14, (s_sound - mainLoop)
    lhz     r5,  (soundTestId - mainLoop)(r14)
    blr

adjItem_sound:
    # r3 = amount to adjust by (0 = A button)
    # r9 = sound effect to play after calling this
    lha     r4,  (soundTestId - mainLoop)(r14)
    add     r4,  r4,  r3
    sth     r4,  (soundTestId - mainLoop)(r14)
    cmpwi   r3,  0
    bnelr
    #mflr   r20
    # r3 is also the ObjInstance* which is null here
    #CALL  audioPlaySound
    #mtlr    r20
    lhz     r9,  (soundTestId - mainLoop)(r14)
    blr

######################################################################

drawItem_music:
    addi    r4,  r14, (s_music - mainLoop)
    lbz     r5,  (musicTestId - mainLoop)(r14)
    LOAD    r6,  0x802c5700 # get song entry
    slwi    r7,  r5,  4 # r7 = r5 * 16
    add     r6,  r6,  r7
    lwz     r6,  4(r6) # get name ptr
    blr

adjItem_music:
    # r3 = amount to adjust by (0 = A button)
    # r9 = sound effect to play after calling this
    lbz     r4,  (musicTestId - mainLoop)(r14)
    #extsb   r4,  r4
    add     r4,  r4,  r3
    cmpwi   r4,  0
    bge     .musicNotMax
    li      r4,  0x63
.musicNotMax:
    cmpwi   r4,  0x64
    blt     .musicNotMin
    li      r4,  0
.musicNotMin:
    stb     r4,  (musicTestId - mainLoop)(r14)
    cmpwi   r3,  0
    bnelr
    mflr    r20
    lbz     r3,  (musicTestId - mainLoop)(r14)
    CALL    0x8000a2e4 # music test function
    mtlr    r20
    li      r9,  0 # no sound effect
    blr

######################################################################

drawItem_streamTest:
    addi    r4,  r14, (s_stream - mainLoop)
    lhz     r5,  (streamTestId - mainLoop)(r14)
    LOADW   r6,  0x803dc850 # STREAMS.bin
    mulli   r7,  r5,  0x16
    add     r6,  r6,  r7
    addi    r6,  r6,  6 # to name
    blr

adjItem_streamTest:
    # r3 = amount to adjust by (0 = A button)
    # r9 = sound effect to play after calling this
    lha     r4,  (streamTestId - mainLoop)(r14)
    add     r4,  r4, r3
    cmpwi   r4,  0
    bge     .streamNotMax
    li      r4,  0x33D
.streamNotMax:
    cmpwi   r4,  0x33E
    blt     .streamNotMin
    li      r4,  0
.streamNotMin:
    sth     r4,  (streamTestId - mainLoop)(r14)
    cmpwi   r3,  0
    bnelr

    mflr    r20
    #CALL    0x8000d594 # reset stream
    CALL    0x8000d01c # stop stream

    lhz     r3,  (streamTestId - mainLoop)(r14)
    LOADW   r6,  0x803dc850 # STREAMS.bin
    mulli   r7,  r3,  0x16
    add     r6,  r6,  r7
    lhz     r3,  0(r6) # get ID
    LOAD    r4,  0x8000d138 # callback the game uses
    CALL    0x8000d200 # play stream
    mtlr    r20
    li      r9,  0 # no sound effect
    blr

inputFuncAudio:
    # r3=buttons, r4=stick X, r5=stick Y,
    # r6=CX, r7=CY, r8=L, r9=R
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r3,  SP_GPR_SAVE(r1)

    andi.   r10, r3,  PAD_BUTTON_Z
    beq     .inputFuncAudio_end

    CALL    0x8000d0c0 # stopAllStreams

    #LOAD    r10, 0x80336d90
    li      r3,  0
    lhz     r4, (soundTestId - mainLoop)(r14)
    #LOADWH  r10, 0x803dc878
    #STOREH  r3,  0x803dc878, r10
    CALL    0x8000b824 # objStopSound_

.inputFuncAudio_end:
    lwz     r0,  SP_LR_SAVE(r1)
    mtlr    r0   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1, STACK_SIZE # restore stack ptr
    blr
