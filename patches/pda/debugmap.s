.ascii "debugmap" # 8 byte file ID for debug

drawItem_freeMove:
    addi    r4,  r14, (s_FreeMove - mainLoop)
    addi    r5,  r14, (s_off - mainLoop)
    LOADW   r6,  PATCH_STATE_PTR
    lbz     r6,  ENABLE_FREE_MOVE(r6)
    cmpwi   r6,  0
    beq     .drawFreeMove_off
    addi    r5,  r14, (s_on - mainLoop)

.drawFreeMove_off:
    blr

adjItem_freeMove: # r3 = amount to adjust by
    LOADW   r3,  PATCH_STATE_PTR
    lbz     r4,  ENABLE_FREE_MOVE(r3)
    xori    r4,  r4, 1
    stb     r4,  ENABLE_FREE_MOVE(r3)
    blr

#######################################################################

drawItem_reloadMap:
    addi    r4,  r14, (s_ReloadMap - mainLoop)
    blr

adjItem_reloadMap: # r3 = amount to adjust by (0=A button)
    cmpwi   r3,  0
    bnelr
    JUMP    0x80020748, r3 # mapReload

#######################################################################

drawItem_warp:
    addi    r4,  r14, (s_Warp - mainLoop)
    blr

adjItem_warp: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_ID_WARP
    b       .debug_setMenu

#######################################################################

.if 0 # this is bugged
drawItem_debugObjs:
    addi    r4,  r14, (s_DebugObjs - mainLoop)
    addi    r5,  r14, (s_off - mainLoop)
    LOADW   r6,  PATCH_STATE_PTR
    lbz     r6,  SHOW_DEBUG_OBJS(r6)
    cmpwi   r6,  0
    beq     .drawDebugObjs_off
    addi    r5,  r14, (s_on - mainLoop)

.drawDebugObjs_off:
    blr

adjItem_debugObjs: # r3 = amount to adjust by (0=A button)
    LOADW   r3,  PATCH_STATE_PTR
    lbz     r4,  SHOW_DEBUG_OBJS(r3)
    xori    r4,  r4,  1
    stb     r4,  SHOW_DEBUG_OBJS(r3)
    blr
.endif

#######################################################################

drawItem_objList:
    addi    r4,  r14, (s_ObjList - mainLoop)
    blr

adjItem_objList: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_ID_OBJLIST
.debug_setMenu:
    cmpwi   r3,  0
    bnelr
    stb     r4,  (whichMenu - mainLoop)(r14)
    blr

#######################################################################

drawItem_spawn:
    addi    r4,  r14, (s_spawn - mainLoop)
    blr

adjItem_spawn: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_ID_SPAWN
    b       .debug_setMenu

#######################################################################
