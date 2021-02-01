.ascii "dbgtext " # 8 byte file ID for debug

drawItem_debugText_master:
    addi    r4,  r14, (s_Master - mainLoop)
    addi    r5,  r14, (s_off - mainLoop)
    LOADB   r6,  enableDebugText
    cmpwi   r6,  0
debugText_printOffOn:
    beqlr
    addi    r5,  r14, (s_on - mainLoop)
    blr

adjItem_debugText_master: # r3 = amount to adjust by (0=A button)
    LOADWH  r3,  enableDebugText
    LOADBL2 r4,  enableDebugText, r3
    xori    r4,  r4,  1
    STOREB  r4,  enableDebugText, r3
    blr

#######################################################################

drawItem_debugText_tricky:
    addi    r4,  r14, (s_Tricky - mainLoop)
    addi    r5,  r14, (s_off - mainLoop)
    LOADB   r6,  0x80148bc8
    cmpwi   r6,  0x94
    b       debugText_printOffOn

adjItem_debugText_tricky: # r3 = amount to adjust by (0=A button)
    LOAD    r3,  0x80148bc8
    lwz     r4,  0(r3)
    lwz     r5,  (.trickyDebugXor - mainLoop)(r14)
    xor     r4,  r4,  r5
    stw     r4,  0(r3)
    li      r4,  0
    icbi    r4,  r3 # flush instruction cache
    blr

# original value is 0x9421FF90: stwu r1, -0x70(r1)
# patched  value is 0x4BFEED80: b 0x80137948
# this value is original XOR patched. by XORing the value
# with this constant, we toggle between the two.
.trickyDebugXor: .int 0xdfdf1210

#######################################################################

drawItem_debugText_playerState:
    addi    r4,  r14, (s_PlayerState - mainLoop)
    li      r8,  DEBUG_TEXT_PLAYER_STATE
debugText_drawItem:
    addi    r5,  r14, (s_off - mainLoop)
    LOADW   r7,  PATCH_STATE_PTR
    lbz     r6,  DEBUG_TEXT_FLAGS(r7)
    and.    r6,  r6,  r8
    b       debugText_printOffOn

adjItem_debugText_playerState: # r3 = amount to adjust by (0=A button)
    li      r7,  DEBUG_TEXT_PLAYER_STATE
debugText_toggleItem:
    LOADW   r5,  PATCH_STATE_PTR
    lbz     r6,  DEBUG_TEXT_FLAGS(r5)
    xor     r6,  r6,  r7
    stb     r6,  DEBUG_TEXT_FLAGS(r5)
    blr

##########################################################################

drawItem_debugText_seqState:
    addi    r4,  r14, (s_SeqState - mainLoop)
    li      r8,  DEBUG_TEXT_SEQ_STATE
    b       debugText_drawItem

adjItem_debugText_seqState: # r3 = amount to adjust by (0=A button)
    li      r7,  DEBUG_TEXT_SEQ_STATE
    b       debugText_toggleItem

##########################################################################

drawItem_debugText_heaps:
    addi    r4,  r14, (s_HeapState - mainLoop)
    li      r8,  DEBUG_TEXT_HEAP_STATE
    b       debugText_drawItem

adjItem_debugText_heaps: # r3 = amount to adjust by (0=A button)
    li      r7,  DEBUG_TEXT_HEAP_STATE
    b       debugText_toggleItem

##########################################################################

drawItem_debugText_restartPoint:
    addi    r4,  r14, (s_RestartPoint - mainLoop)
    li      r8,  DEBUG_TEXT_RESTART_POINT
    b       debugText_drawItem

adjItem_debugText_restartPoint: # r3 = amount to adjust by (0=A button)
    li      r7,  DEBUG_TEXT_RESTART_POINT
    b       debugText_toggleItem

##########################################################################
