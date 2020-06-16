# MODELS.tab patch
# Updates MODELS.tab to point to the Krystal model which we
# injected in the MODELS.bin patch.

modelsTab_doPatch:
    # subroutine called from modelsTabPatch
    # to apply the patch to the file slots
    # r3 = models.bin file ID * 4
    # r4 = models.tab file ID * 4
    lwzx    r9,  r3, r11 # r9 = &MODELS.bin
    cmpwi   r9,  0
    beq     .modelsTab_doPatch_end # not loaded, skip

    # get the size of this file
    lwzx    r5, r3, r12 # r5 = size
    # that's the size of the original file, before we appended
    # our model data, so we don't need to adjust it.
    # the original file's size equals the offset of the appended data.
    add     r7, r5, r9 # r7 = the actual address
    lwz     r7, 0(r7)
    cmp     0, 0, r7, r10 # is there a model here?
    # we don't need to check for other valid headers because we're
    # only checking for a specific model
    bne     .modelsTab_doPatch_end

    # OK, r5 is valid.
    oris    r5, r5, 0x8000 # flags: compressed
    lwzx    r9, r4, r11 # r9 = MODELS.tab
    cmpwi   r9, 0
    beq     .modelsTab_doPatch_end # table not loaded!? idk why this happens.
    stw     r5, (KRYSTAL_MODEL_ID*4)(r9) # store the offset

    .modelsTab_doPatch_end:
        blr

modelsTabPatch:
    # patch mergeTableFiles() case MODELS.tab
    # insert the offset we loaded Krystal at into the table,
    # so that the game will actually use the model.
    # patch at 0x80043d7c, "b 0x80043df4" -> "b modelsTabPatch"
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stw     r0, SP_R0_SAVE(r1)
    stmw    r3, SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3, SP_LR_SAVE(r1)

    LOAD    r12, dataFileSize
    addi    r11, r12, dataFileBuffers - dataFileSize
    LOAD    r10, 0xFACEFEED

    # patch main slot
    li      r3, MODELS_BIN*4
    li      r4, MODELS_TAB*4
    bl      modelsTab_doPatch

    # patch secondary slot
    li      r3, MODELS_BIN2*4
    li      r4, MODELS_TAB2*4
    bl      modelsTab_doPatch

    # restore registers
endTabPatch:
    lwz     r0, SP_R0_SAVE(r1)
    lwz     r3, SP_LR_SAVE(r1)
    mtlr    r3
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr

    # jump back to game code
    # the instruction we replaced was also a branch to here.
    JUMP    0x80043df4, r11
