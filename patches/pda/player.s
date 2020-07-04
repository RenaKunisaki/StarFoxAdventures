drawItem_player:
    addi    r4, r14, (s_Player - mainLoop)
    addi    r5, r14, (s_Krystal - mainLoop)
    LOADW   r6, PATCH_STATE_PTR
    lbz     r6, CUR_CHAR_ADDR(r6)
    andi.   r6, r6, 0x7F
    cmpwi   r6, 0
    beq     .drawPlayer_krystal
    addi    r5, r14, (s_Fox - mainLoop)
.drawPlayer_krystal:
    blr

adjItem_player: # r3 = amount to adjust by
    LOADW   r5, PATCH_STATE_PTR
    lbz     r6, CUR_CHAR_ADDR(r5)
    xori    r6, r6, 1
    stb     r6, CUR_CHAR_ADDR(r5)
    blr
