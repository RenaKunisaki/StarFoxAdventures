.text
.include "common.s"

# patch mergeTableFiles() case TEX1.tab
# insert the offset we loaded Krystal at into the table
# nearly identical to patch 2
GECKO_BEGIN_PATCH 0x80043db4 # li r9, 0x1000
b start

.set P_TEX1_TAB1,dataFileBuffers + (TEX1_TAB  * 4)
.set P_TEX1_TAB2,dataFileBuffers + (TEX1_TAB2 * 4)
.set P_TEX1_BIN1,dataFileBuffers + (TEX1_BIN  * 4)
.set P_TEX1_BIN2,dataFileBuffers + (TEX1_BIN2 * 4)
.set NUM_TEXTURES,7
.set BASE_OFFSET,0x023430 # first entry
.set BASE_ID,0x724 # first ID to patch in table
.set LAST_ID,BASE_ID+NUM_TEXTURES

.set STACK_SIZE,0x100 # how much to reserve
.set SP_LR_SAVE,0x104 # this is what the game does
.set SP_GPR_SAVE,0x10

offsets: # for quick access
    .int dataFileSize

textureData:
    # copied from TEX1.tab and offset by the first entry.
    .int 0x84023430 - BASE_OFFSET
    .int 0x810261E0 - BASE_OFFSET
    .int 0x8102F1B0 - BASE_OFFSET
    .int 0x01000000
    .int 0x8102FB10 - BASE_OFFSET
    .int 0x8102FEA0 - BASE_OFFSET
    .int 0x8102FFE0 - BASE_OFFSET


do_patch:
    # r3 = tex1.bin file ID * 4
    # r4 = tex1.tab file ID * 4
    lwzx    r9,  r3, r11 # r9 = &TEX1.bin
    cmpwi   r9,  0
    beq     .end # not loaded, skip

    # get the size of this file
    lwzx    r5, r3, r12 # r5 = size
    # that's the size of the original file, before we appended
    # our texture data, so we don't need to adjust it.
    addi    r5, r5, 0x20 # except this for some reason

    add     r7, r5, r9 # r7 = the actual address
    lwz     r7, 0(r7)
    cmpwi   r7, 0x14 # is our texture here?
    bne     .end

    # OK, r5 is valid.

    srwi    r5, r5, 1 # divide by 2 as the game expects
    lwzx    r9, r4, r11 # r9 = TEX1.tab
    cmpwi   r9, 0
    beq     .end # table not loaded!? idk why this happens.
    #stw     r5, (KRYSTAL_MODEL_ID*4)(r9) # store the offset

    li      r3, BASE_ID
    mr      r4, r10
    .next:
        lwzu    r6, 4(r4)  # r6 = offset

        # there's a gap in the middle of the IDs.
        # we detect this by it beginning with 0x01
        # instead of 0x8x. we don't want to change
        # that entry at all.
        srwi    r7, r6, 24
        cmpwi   r7, 1
        beq     .skip

        # add the offset we loaded the texture to,
        # and the offset from our table above.
        add     r6, r6, r5 # plus the offset we wrote it at
        slwi    r7, r3,  2 # ID -> offset
        stwx    r6, r7, r9 # store the offset

    .skip:
        addi    r3, r3,  1
        cmpwi   r3, LAST_ID
        bne     .next

    .end:
        blr


start:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stmw    r3, SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3, SP_LR_SAVE(r1)


    # get table address in r5
    mflr r7
    bl   .getpc
    .getpc:
        mflr r10
        mtlr r7 # restore LR
        # subtract 4 so we can use lwzu
        addi r10, r10, ((offsets - .getpc)-4)@l

    lwzu    r12, 4(r10) # r12 = dataFileSize
    addi    r11, r12, dataFileBuffers - dataFileSize

    li      r3, TEX1_BIN*4
    li      r4, TEX1_TAB*4
    bl      do_patch

    li      r3, TEX1_BIN2*4
    li      r4, TEX1_TAB2*4
    bl      do_patch

    lwz     r3, SP_LR_SAVE(r1)
    mtlr    r3
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    li      r9, 0x1000 # replaced

GECKO_END_PATCH
