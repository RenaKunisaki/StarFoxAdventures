.ascii "gametext" # 8 byte file ID for debug

drawItem_debugGameText:
    addi    r4,  r14, s_GameText - mainLoop
    blr

adjItem_debugGameText: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_PAGE_DEBUG_GAMETEXT
    b       .debugSetPage

#######################################################################

drawItem_textTest:
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r5
    stw     r5,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r21, r3
    lhz     r3,  (textTestBox - mainLoop)(r14)
    CALL    0x80019570 # gameTextGet
    cmpwi   r3,  0
    beq     .drawItem_textTest_noDraw
    lhz     r3,  (textTestBox - mainLoop)(r14)
    CALL    0x800173e4 # gameTextSelectBox
    lhz     r3,  (textTestId - mainLoop)(r14)
    CALL    0x80016870

.drawItem_textTest_noDraw:
    mtlr    r20
    mr      r3,  r21

    addi    r4,  r14, (s_TextTest - mainLoop)
    lhz     r5,  (textTestId - mainLoop)(r14)
    b       menuEndSub

adjItem_textTest:
    # r3 = amount to adjust by (0 = A button)
    # r9 = sound effect to play after calling this
    lha     r4,  (textTestId - mainLoop)(r14)
    add     r4,  r4,  r3
    sth     r4,  (textTestId - mainLoop)(r14)
    blr

#######################################################################

drawItem_gameTextBox:
    addi    r4,  r14, (s_TextBox - mainLoop)
    lhz     r5,  (textTestBox - mainLoop)(r14)
    blr

adjItem_gameTextBox: # r3 = amount to adjust by (0=A button)
    lha     r4,  (textTestBox - mainLoop)(r14)
    add     r4,  r4,  r3
    sth     r4,  (textTestBox - mainLoop)(r14)
    blr
