drawItem_debugText_master:
    addi    r4,  r14, (s_Master - mainLoop)
    addi    r5,  r14, (s_off - mainLoop)
    LOADB   r6,  enableDebugText
    cmpwi   r6,  0
    beq     .drawDebugText_off
    addi    r5,  r14, (s_on - mainLoop)

.drawDebugText_off:
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
    cmpwi   r6,  0x4B
    bne     .drawTrickyDebug_off
    addi    r5,  r14, (s_on - mainLoop)

.drawTrickyDebug_off:
    blr

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
