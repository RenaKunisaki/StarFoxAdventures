drawItem_debugText:
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

#######################################################################

drawItem_trickyDebug:
    addi  r4, r14, (s_TrickyDebug - mainLoop)
    addi  r5, r14, (s_off - mainLoop)
    LOADB r6, 0x80148bc8
    cmpwi r6, 0x4B
    bne   .drawDebugText_off
    addi  r5, r14, (s_on - mainLoop)

.drawTrickyDebug_off:
    blr

adjItem_trickyDebug: # r3 = amount to adjust by
    LOAD    r3, 0x80148bc8
    lwz     r4, 0(r3)
    lwz     r5, (.trickyDebugXor - mainLoop)(r14)
    xor     r4, r4, r5
    stw     r4, 0(r3)
    li      r4, 0
    icbi    r4, r3 # flush instruction cache
    blr

# original value is 0x9421FF90: stwu r1, -0x70(r1)
# patched  value is 0x4BFEED80: b 0x80137948
# this value is original XOR patched. by XORing the value
# with this constant, we toggle between the two.
.trickyDebugXor: .int 0xdfdf1210

#######################################################################

drawItem_freeMove:
    addi  r4, r14, (s_FreeMove - mainLoop)
    addi  r5, r14, (s_off - mainLoop)
    LOADB r6, ENABLE_FREE_MOVE
    cmpwi r6, 0
    beq   .drawFreeMove_off
    addi  r5, r14, (s_on - mainLoop)

.drawFreeMove_off:
    blr

adjItem_freeMove: # r3 = amount to adjust by
    LOADWH  r3, ENABLE_FREE_MOVE
    LOADBL2 r4, ENABLE_FREE_MOVE, r3
    xori    r4, r4, 1
    STOREB  r4, ENABLE_FREE_MOVE, r3
    blr

#######################################################################

drawItem_reloadMap:
    addi  r4, r14, (s_ReloadMap - mainLoop)
    blr

adjItem_reloadMap: # r3 = amount to adjust by (0=A button)
    cmpwi   r3, 0
    bnelr
    JUMP    0x80020748, r3 # mapReload

#######################################################################

drawItem_objects:
    addi  r4, r14, (s_Objects - mainLoop)
    blr

adjItem_objects: # r3 = amount to adjust by (0=A button)
    cmpwi   r3, 0
    bnelr
    li      r3, MENU_ID_OBJLIST
    stb     r3, (whichMenu - mainLoop)(r14)
    blr

#######################################################################

drawItem_gameBits:
    addi  r4, r14, (s_GameBits - mainLoop)
    blr

adjItem_gameBits: # r3 = amount to adjust by (0=A button)
    cmpwi   r3, 0
    bnelr
    li      r3, MENU_ID_GAMEBIT
    stb     r3, (whichMenu - mainLoop)(r14)
    blr
