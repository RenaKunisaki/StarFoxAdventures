.ascii "debugseq" # 8 byte file ID for debug

drawItem_debugSeq:
    addi    r4,  r14, (s_ObjSeq - mainLoop)
    blr

adjItem_debugSeq: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_PAGE_DEBUG_SEQ
    b       .debugSetPage


#######################################################################

drawItem_playSeq:
    addi    r4,  r14, (s_PlaySeq - mainLoop)
    lhz     r5,  (debugSeqId - mainLoop)(r14)
    blr

adjItem_playSeq: # r3 = amount to adjust by (0=A button)
    # r9 = sound effect to play after calling this
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r3,  SP_GPR_SAVE(r1)

    cmpwi   r3,  0
    beq     .adjItem_playSeq_doCall
    lhz     r4,  (debugSeqId - mainLoop)(r14)
    add     r4,  r4,  r3
    sth     r4,  (debugSeqId - mainLoop)(r14)
    b       .adjItem_playSeq_end

.adjItem_playSeq_doCall:
    lhz     r3,  (debugSeqId - mainLoop)(r14)
    LOADW   r4,  pPlayer
    LOAD    r5,  0xFFFFFFFF
    CALL    0x80080de8

.adjItem_playSeq_end:
    lwz     r0,  SP_LR_SAVE(r1)
    mtlr    r0   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1, STACK_SIZE # restore stack ptr

    blr

#######################################################################

drawItem_stopSeq:
    addi    r4,  r14, (s_StopSeq - mainLoop)
    LOADH   r6,  seqGlobal1
    blr

adjItem_stopSeq: # r3 = amount to adjust by (0=A button)
    cmpwi   r3,  0
    beqlr
    lhz     r3,  (debugSeqId - mainLoop)(r14)
    JUMP    0x80080c18, r0

#######################################################################

drawItem_editSeq:
    addi    r4,  r14, (s_EditSeq - mainLoop)
    lbz     r5,  (debugSeqIdx - mainLoop)(r14)
    blr

adjItem_editSeq: # r3 = amount to adjust by (0=A button)
    lbz     r4,  (debugSeqIdx - mainLoop)(r14)
    add     r4,  r4,  r3
    cmpwi   r4,  0
    blt     .adjItem_editSeq_wrapMax
    cmpwi   r4,  0x54
    ble     .adjItem_editSeq_noWrap
    li      r4,  0
.adjItem_editSeq_noWrap:
    stb     r4,  (debugSeqIdx - mainLoop)(r14)
    blr
.adjItem_editSeq_wrapMax:
    li      r4,  0x54
    b       .adjItem_editSeq_noWrap

#######################################################################

drawItem_seqGlobal1:
    addi    r4,  r14, (s_GlobalN - mainLoop)
    li      r5,  1
    LOADH   r6,  seqGlobal1
    blr

adjItem_seqGlobal1: # r3 = amount to adjust by (0=A button)
    LOADWH  r5,  seqGlobal1
    LOADHL2 r4,  seqGlobal1, r5
    add     r4,  r4,  r3
    STOREH  r4,  seqGlobal1, r5
    blr

#######################################################################

drawItem_seqGlobal2:
    addi    r4,  r14, (s_GlobalN - mainLoop)
    li      r5,  2
    LOADH   r6,  seqGlobal2
    blr

adjItem_seqGlobal2: # r3 = amount to adjust by (0=A button)
    LOADWH  r5,  seqGlobal2
    LOADHL2 r4,  seqGlobal2, r5
    add     r4,  r4,  r3
    STOREH  r4,  seqGlobal2, r5
    blr

#######################################################################

drawItem_seqGlobal3:
    addi    r4,  r14, (s_GlobalN - mainLoop)
    li      r5,  2
    LOADB   r6,  seqGlobal3
    blr

adjItem_seqGlobal3: # r3 = amount to adjust by (0=A button)
    LOADWH  r5,  seqGlobal3
    LOADBL2 r4,  seqGlobal3, r5
    add     r4,  r4,  r3
    STOREB  r4,  seqGlobal3, r5
    blr

#######################################################################

drawItem_seqBool:
    addi    r4,  r14, (s_SeqBool - mainLoop)
    LOAD    r5,  objSeqBool
    lbz     r6,  (debugSeqIdx - mainLoop)(r14)
    lbzx    r5,  r5,  r6
    blr

adjItem_seqBool: # r3 = amount to adjust by (0=A button)
    LOAD    r5,  objSeqBool
    lbz     r6,  (debugSeqIdx - mainLoop)(r14)
    lbzx    r4,  r5,  r6
    xori    r4,  r4,  1
    stbx    r4,  r5,  r6
    blr

#######################################################################

drawItem_seqVar1:
    addi    r4,  r14, (s_SeqVar - mainLoop)
    LOAD    r5,  objSeqVar1
    lbz     r6,  (debugSeqIdx - mainLoop)(r14)
    lbzx    r6,  r5,  r6
    li      r5,  1
    blr

adjItem_seqVar1: # r3 = amount to adjust by (0=A button)
    LOAD    r5,  objSeqVar1
    lbz     r6,  (debugSeqIdx - mainLoop)(r14)
    lbzx    r4,  r5,  r6
    add     r4,  r4,  r3
    stbx    r4,  r5,  r6
    blr

#######################################################################

drawItem_seqVar2:
    addi    r4,  r14, (s_SeqVar - mainLoop)
    LOAD    r5,  objSeqVar2
    lbz     r6,  (debugSeqIdx - mainLoop)(r14)
    lbzx    r6,  r5,  r6
    li      r5,  2
    blr

adjItem_seqVar2: # r3 = amount to adjust by (0=A button)
    LOAD    r5,  objSeqVar2
    lbz     r6,  (debugSeqIdx - mainLoop)(r14)
    lbzx    r4,  r5,  r6
    add     r4,  r4,  r3
    stbx    r4,  r5,  r6
    blr

#######################################################################

drawItem_seqVar3:
    addi    r4,  r14, (s_SeqVar - mainLoop)
    LOAD    r5,  objSeqVar3
    lbz     r6,  (debugSeqIdx - mainLoop)(r14)
    lbzx    r6,  r5,  r6
    li      r5,  3
    blr

adjItem_seqVar3: # r3 = amount to adjust by (0=A button)
    LOAD    r5,  objSeqVar3
    lbz     r6,  (debugSeqIdx - mainLoop)(r14)
    lbzx    r4,  r5,  r6
    add     r4,  r4,  r3
    stbx    r4,  r5,  r6
    blr

#######################################################################

drawItem_seqFlags:
    addi    r4,  r14, (s_SeqFlags - mainLoop)
    LOAD    r5,  objSeqFlags
    lbz     r6,  (debugSeqIdx - mainLoop)(r14)
    lbzx    r5,  r5,  r6
    blr

adjItem_seqFlags: # r3 = amount to adjust by (0=A button)
    LOAD    r5,  objSeqFlags
    lbz     r6,  (debugSeqIdx - mainLoop)(r14)
    lbzx    r4,  r5,  r6
    add     r4,  r4,  r3
    stbx    r4,  r5,  r6
    blr

#######################################################################
