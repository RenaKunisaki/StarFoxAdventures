.ascii "dbgaudio" # 8 byte file ID for debug

drawItem_debugAudio:
    addi    r4,  r14, (s_Audio - mainLoop)
    blr

adjItem_debugAudio: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_PAGE_DEBUG_AUDIO
    b       .debugSetPage

#######################################################################

inputFuncDebugAudio:
    # r3=buttons, r4=stick X, r5=stick Y,
    # r6=CX, r7=CY, r8=L, r9=R
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r3,  SP_GPR_SAVE(r1)

    andi.   r10, r3,  PAD_BUTTON_Z
    beq     .inputFuncDebugAudio_end

    lbz     r6,  (dbgAudioParam - mainLoop)(r14)
    xori    r6,  r6,  1
    stb     r6,  (dbgAudioParam - mainLoop)(r14)

.inputFuncDebugAudio_end:
    lwz     r0,  SP_LR_SAVE(r1)
    mtlr    r0   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1, STACK_SIZE # restore stack ptr
    blr

#######################################################################

drawItem_dbgAudioCmd:
    addi    r4,  r14, (s_DbgAudioCmd - mainLoop)
    lbz     r5,  (dbgAudioCmd - mainLoop)(r14)
    lbz     r6,  (dbgAudioParam - mainLoop)(r14)
    blr

adjItem_dbgAudioCmd: # r3 = amount to adjust by (0=A button)
    # r9 = sound effect to play after calling this
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r3,  SP_GPR_SAVE(r1)

    cmpwi   r3,  0
    beq     .adjItem_dbgAudioCmd_doCall
    lbz     r4,  (dbgAudioCmd - mainLoop)(r14)
    add     r4,  r4,  r3
    stb     r4,  (dbgAudioCmd - mainLoop)(r14)
    b       .adjItem_dbgAudioCmd_end

.adjItem_dbgAudioCmd_doCall:
    lbz     r3,  (dbgAudioCmd - mainLoop)(r14)
    lbz     r4,  (dbgAudioParam - mainLoop)(r14)
    CALL    0x8000a518

.adjItem_dbgAudioCmd_end:
    lwz     r0,  SP_LR_SAVE(r1)
    mtlr    r0   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1, STACK_SIZE # restore stack ptr
    li      r9,  0

    blr

#######################################################################
