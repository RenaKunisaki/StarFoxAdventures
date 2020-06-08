itemHook: # called when the PDA is being toggled on/off by player.
    # r13 = &pdaOn + 0x7630
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r5
    stw   r5, SP_LR_SAVE(r1)
    stmw  r3, SP_GPR_SAVE(r1)

    bl .itemHook_getpc
    .itemHook_getpc:
        mflr r29

    li   r3, 1
    stb  r3, (menuVisible - .itemHook_getpc)(r29)

    lwz  r5, SP_LR_SAVE(r1)
    mtlr r5 # restore LR
    lmw  r3, SP_GPR_SAVE(r1)
    addi r1, r1, STACK_SIZE # restore stack ptr

    JUMP 0x80133A94, r0 # skip the actual on/off code
