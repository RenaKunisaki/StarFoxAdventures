drawItem_debugText: # r3 = strBuf
    addi  r4, r14, (s_DebugText - mainLoop)
    addi  r5, r14, (s_off - mainLoop)
    LOADB r6, enableDebugText
    cmpwi r6, 0
    beq   .drawDebugText_off
    addi  r5, r14, (s_on - mainLoop)

.drawDebugText_off:
    blr

adjItem_debugText: # r3 = amount to adjust by
    LOADWH  r3, enableDebugText
    LOADBL2 r4, enableDebugText, r3
    xori    r4, r4, 1
    STOREB  r4, enableDebugText, r3
    blr
