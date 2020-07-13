# Main Loop patch

mainLoopPatch:
    # r3 = address of mainLoopPatch
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stmw    r3, SP_GPR_SAVE(r1)
    mflr    r15
    stw     r15, SP_LR_SAVE(r1)
    mr      r10, r3
    LOADW   r16, PATCH_STATE_PTR
    lbz     r15, CUR_CHAR_ADDR(r16)
    andi.   r15, r15, 0x7F

    LOADW r3, pPlayer
    cmpwi r3, 0
    beq   .mainLoopPatch_end

    # ensure the player is object ID 1.
    # on the title screen, the Fox object is the player,
    # but it's not actually the same object type as the playable
    # Fox, and trying to change its model ID will crash the game.
    # it's 0x30
    lhz   r4, 0x44(r3)
    cmpwi r4, 1
    bne   .mainLoopPatch_end

    # don't apply when using SharpClaw disguise
    lbz   r4, 0xAD(r3)
    cmpwi r4, 2
    beq   .mainLoopPatch_end

    # set correct model for selected character.
    stb   r15, 0xAD(r3) # model index
    lwz   r4,  0xB8(r3)
    stb   r15, 0x081B(r4) # voice and backpack

.mainLoopPatch_end:
    lwz     r3, SP_LR_SAVE(r1)
    mtlr    r3
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    blr


.if 0
settingsPatch:
    cmpwi  r3, 0
    bne    .settingsPatch_disabled
    li     r3, 0x80
.settingsPatch_disabled:
    LOADW   r16, PATCH_STATE_PTR
    stb     r3, CUR_CHAR_ADDR(r16)
    JUMP    0x8011C4BC, r0


titlePatch:
    mflr  r14
    CALL  0x80019970 # replaced
    li    r3, 0
    CALL  gameTextGet
    cmpwi r3, 0
    beq   .titlePatch_end

    lwz   r3, 8(r3)
    cmpwi r3, 0
    beq   .titlePatch_end

    lwz   r3, (0x366 * 4)(r3)
    cmpwi r3, 0
    beq   .titlePatch_end

    bl    .titlePatch_getpc
    .titlePatch_getpc: mflr r5
    addi  r4, r5, titleData - .titlePatch_getpc
    li    r5, 8
    CALL  strncpy


.titlePatch_end:
    mtlr r14
    blr
.endif
