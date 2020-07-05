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
    bne   .drawTrickyDebug_off
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

.if 0 # this is bugged
drawItem_debugObjs:
    addi    r4, r14, (s_DebugObjs - mainLoop)
    addi    r5, r14, (s_off - mainLoop)
    LOADW   r6, PATCH_STATE_PTR
    lbz     r6, SHOW_DEBUG_OBJS(r6)
    cmpwi   r6, 0
    beq     .drawDebugObjs_off
    addi    r5, r14, (s_on - mainLoop)

.drawDebugObjs_off:
    blr

adjItem_debugObjs: # r3 = amount to adjust by
    LOADW   r3, PATCH_STATE_PTR
    lbz     r4, SHOW_DEBUG_OBJS(r3)
    xori    r4, r4, 1
    stb     r4, SHOW_DEBUG_OBJS(r3)
    blr
.endif

#######################################################################

drawItem_freeMove:
    addi    r4, r14, (s_FreeMove - mainLoop)
    addi    r5, r14, (s_off - mainLoop)
    LOADW   r6, PATCH_STATE_PTR
    lbz     r6, ENABLE_FREE_MOVE(r6)
    cmpwi   r6, 0
    beq     .drawFreeMove_off
    addi    r5, r14, (s_on - mainLoop)

.drawFreeMove_off:
    blr

adjItem_freeMove: # r3 = amount to adjust by
    LOADW   r3, PATCH_STATE_PTR
    lbz     r4, ENABLE_FREE_MOVE(r3)
    xori    r4, r4, 1
    stb     r4, ENABLE_FREE_MOVE(r3)
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
    li      r4, MENU_ID_OBJLIST
.debug_setMenu:
    cmpwi   r3, 0
    bnelr
    stb     r4, (whichMenu - mainLoop)(r14)
    blr

#######################################################################

drawItem_gameBits:
    addi  r4, r14, (s_GameBits - mainLoop)
    blr

adjItem_gameBits: # r3 = amount to adjust by (0=A button)
    li      r4, MENU_ID_GAMEBIT
    b       .debug_setMenu

#######################################################################

drawItem_saveGame:
    addi  r4, r14, (s_SaveGame - mainLoop)
    blr

adjItem_saveGame: # r3 = amount to adjust by (0=A button)
    li      r4, MENU_ID_SAVEGAME
    b       .debug_setMenu

#######################################################################

drawItem_warp:
    addi  r4, r14, (s_Warp - mainLoop)
    blr

adjItem_warp: # r3 = amount to adjust by (0=A button)
    li      r4, MENU_ID_WARP
    b       .debug_setMenu

#######################################################################

drawItem_heap:
    addi  r4, r14, (s_Heap - mainLoop)
    blr

adjItem_heap: # r3 = amount to adjust by (0=A button)
    li      r4, MENU_ID_HEAP_LIST
    b       .debug_setMenu

#######################################################################

drawItem_textTest:
    addi r4, r14, (s_TextTest - mainLoop)
    lhz  r5, (textTestId - mainLoop)(r14)
    blr

adjItem_textTest:
    # r3 = amount to adjust by (0 = A button)
    # r9 = sound effect to play after calling this
    lha   r4, (textTestId - mainLoop)(r14)
    add   r4, r4, r3
    sth   r4, (textTestId - mainLoop)(r14)
    cmpwi r3, 0
    bnelr
    lhz   r3, (textTestId - mainLoop)(r14)
    li    r6, 1 # hide HUD and lock player
    mflr  r20
    CALL  0x8012ea5c # params r4, r5 are unused
    mtlr  r20
    blr

#######################################################################

drawItem_frameAdvance:
    addi  r4, r14, (s_frameAdv - mainLoop)
    blr

adjItem_frameAdvance: # r3 = amount to adjust by (0=A button)
    li     r4, 0
    LOADWH r5, timeStop
    STOREB r4, timeStop, r5
    blr

#######################################################################

# temporary item
#drawItem_Save:
#    addi  r4, r14, (s_Save - mainLoop)
#    blr
#
#adjItem_Save: # r3 = amount to adjust by (0=A button)
#    JUMP 0x800e86d0, r0
    # this works but pops up the saving message
    # looks like there's not much way to bypass that except
    # to patch the save function
    # NOPs at 8007db50, 8007db94 seem to be safe
    # but when should we do the save? on loading a map?
