.ascii "dbgrendr" # 8 byte file ID for debug

drawItem_textureDebug:
    addi    r4,  r14, (s_Textures - mainLoop)
    blr

adjItem_textureDebug: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_ID_TEXTURES
    b       .debug_setMenu

#######################################################################

drawItem_hitboxes:
    addi    r4,  r14, (s_Hitboxes - mainLoop)
    addi    r5,  r14, (s_off - mainLoop)
    LOADW   r6,  PATCH_STATE_PTR
    lbz     r6,  DEBUG_RENDER_FLAGS(r6)
    andi.   r6,  r6,  DEBUG_RENDER_HITBOXES
    beq     .drawItem_hitboxes_off
    addi    r5,  r14, (s_on - mainLoop)

.drawItem_hitboxes_off:
    blr

adjItem_hitboxes: # r3 = amount to adjust by (0=A button)
    LOADW   r3,  PATCH_STATE_PTR
    lbz     r4,  DEBUG_RENDER_FLAGS(r3)
    xori    r4,  r4,  DEBUG_RENDER_HITBOXES
    stb     r4,  DEBUG_RENDER_FLAGS(r3)
    blr

#######################################################################
