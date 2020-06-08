drawItem_player:
    addi    r4, r14, (s_Player - mainLoop)
    addi    r5, r14, (s_Krystal - mainLoop)
    LOADWH  r6, CUR_CHAR_ADDR
    LOADBL2 r6, CUR_CHAR_ADDR, r6
    cmpwi   r6, 0
    beq     .drawPlayer_krystal
    addi    r5, r14, (s_Fox - mainLoop)
.drawPlayer_krystal:
    blr

adjItem_player: # r3 = amount to adjust by
    LOADWH  r5, CUR_CHAR_ADDR
    LOADBL2 r6, CUR_CHAR_ADDR, r5
    xori    r6, r6, 1
    STOREB  r6, CUR_CHAR_ADDR, r5
    blr
