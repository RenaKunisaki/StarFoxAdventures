# TEX1.tab patch
# Updates TEX1.tab to reference the Krystal textures that we
# inject in the TEX1.bin patch.

tex1_doPatch:
    # r3 = tex1.bin file ID * 4
    # r4 = tex1.tab file ID * 4
    lwzx    r9,  r3, r11 # r9 = &TEX1.bin
    cmpwi   r9,  0
    beq     .tex1_doPatch_end # not loaded, skip

    # get the size of this file
    lwzx    r5, r3, r12 # r5 = size
    # that's the size of the original file, before we appended
    # our texture data, so we don't need to adjust it.
    addi    r5, r5, 0x20 # except this for some reason

    add     r7, r5, r9 # r7 = the actual address
    lwz     r7, 0(r7)
    cmpwi   r7, 0x14 # is our texture here?
    bne     .tex1_doPatch_end

    # OK, r5 is valid.

    srwi    r5, r5, 1 # divide by 2 as the game expects
    lwzx    r9, r4, r11 # r9 = TEX1.tab
    cmpwi   r9, 0
    beq     .tex1_doPatch_end # table not loaded!? idk why this happens.

    li      r3, TEX1_BASE_ID
    mr      r4, r14
.tex1_doPatch_next:
    lwzu    r6, 4(r4)  # r6 = offset

    # there's a gap in the middle of the IDs.
    # we detect this by it beginning with 0x01
    # instead of 0x8x. we don't want to change
    # that entry at all.
    srwi    r7, r6, 24
    cmpwi   r7, 1
    beq     .tex1_doPatch_skip

.if DEBUG
    mflr    r20
    mr      r15, r3
    mr      r16, r4
    mr      r17, r5
    mr      r18, r6
    mr      r19, r9
    addi    r3, r30, msg_doTex1TabPatch - .tex1Tab_getpc
    mr      r4, r15
    mr      r5, r6
    CALL    OSReport
    mr      r3,  r15
    mr      r4,  r16
    mr      r5,  r17
    mr      r6,  r18
    mr      r9,  r19
    mtlr    r20
.endif

    # add the offset we loaded the texture to,
    # and the offset from our table below.
    add     r6, r6, r5 # plus the offset we wrote it at
    slwi    r7, r3,  2 # ID -> offset
    stwx    r6, r7, r9 # store the offset

.tex1_doPatch_skip:
    addi    r3, r3,  1
    cmpwi   r3, TEX1_LAST_ID
    bne     .tex1_doPatch_next

.tex1_doPatch_end:
    blr


tex1TabPatch:
    # patch mergeTableFiles() case TEX1.tab
    # insert the offset we loaded Krystal at into the table,
    # so that the game will actually use the textures.
    # nearly identical to modelsTabPatch.
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stw     r0, SP_R0_SAVE(r1)
    stmw    r3, SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3, SP_LR_SAVE(r1)

    bl   .tex1Tab_getpc
    .tex1Tab_getpc:
        mflr r30
    mr      r14, r30

    LOADW   r4, PATCH_STATE_PTR
    lbz     r3, CUR_CHAR_ADDR(r4)
    srwi    r3, r3, 7

.if DEBUG
    mr      r15, r3
    addi    r3, r14, msg_whichTex1Tab - .tex1Tab_getpc
    mr      r4, r15
    CALL    OSReport
    mr      r3,  r15
.endif

    cmpwi   r3, 0
    beq     .tex1Tab_noCheat
    addi    r14, r14, (tex1Tab_data2 - .tex1Tab_getpc)-4
    b       .tex1Tab_doPatch

.tex1Tab_noCheat:
    # subtract 4 so we can use lwzu
    addi    r14, r14, (tex1Tab_data1 - .tex1Tab_getpc)-4

.tex1Tab_doPatch:
    lwzu    r12, 4(r14) # r12 = dataFileSize
    addi    r11, r12, dataFileBuffers - dataFileSize

    li      r3, TEX1_BIN*4
    li      r4, TEX1_TAB*4
    bl      tex1_doPatch

    li      r3, TEX1_BIN2*4
    li      r4, TEX1_TAB2*4
    bl      tex1_doPatch
    b       endTabPatch # reuse code

tex1Tab_data1:
    .int dataFileSize # for quick access
    # copied from TEX1.tab and offset by the first entry.
    .int 0x84000000 # eye texture
    .int 0x81002DB0 # fur texture
    .int 0x8100BD80 # ? maybe hair sides?
    .int 0x01000000
    .int 0x8100C6E0 # another eye texture?
    .int 0x8100CA70 # hair front
    .int 0x8100CBB0 # hair back

tex1Tab_data2: # for alt
    .int dataFileSize # for quick access
    .int 0x84000000
    .int 0x81002DB0
    .int 0x81012308
    .int 0x01000000
    .int 0x81012C68
    .int 0x81012FF8
    .int 0x81013138
