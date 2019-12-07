.text
.include "common.s"

# patch mergeTableFiles() case TEX1.tab
# insert the offset we loaded Krystal at into the table
# nearly identical to patch 2
GECKO_BEGIN_PATCH 0x80043db4 # li r9, 0x1000
b start

.set TEX1_TAB_MAIN,0x8035f46c
.set TEX1_TAB_SUB,0x8035f518
.set NUM_TEXTURES,7
.set BASE_OFFSET,0x023430 # first entry
.set BASE_ID,0x724 # first ID to patch in table
.set LAST_ID,BASE_ID+NUM_TEXTURES
.set STACK_SIZE,0x100 # how much to reserve
.set SP_LR_SAVE,0x10
.set SP_GPR_SAVE,0x14

offsets:
    .int TEX1_TAB_MAIN, TEX1_TAB_SUB, 0x3FFFFFFF, 0x40000000

textureData:
    # copied from TEX1.tab and offset by the first entry.
    .int 0x84023430 - BASE_OFFSET
    .int 0x810261E0 - BASE_OFFSET
    .int 0x8102F1B0 - BASE_OFFSET
    .int 0x01000000
    .int 0x8102FB10 - BASE_OFFSET
    .int 0x8102FEA0 - BASE_OFFSET
    .int 0x8102FFE0 - BASE_OFFSET

# using .align generates unnecessary extra padding.
#.byte 0, 0
do_patch:
    # get the texture offset, stored by previous code.
    lis     r4, 0x8180
    lwz     r4, -8(r4)
    cmpwi   r4, 0
    beq     abort # we didn't load the texture.
    # better to let it be a rainbow than to crash the game

    srwi    r4, r4, 1 # divide by 2 as the game expects

    # get table address in r5
    mflr r7
    bl   .getpc
    .getpc:
        mflr r5
        mtlr r7 # restore LR
        # subtract 4 so we can use lwzu
        addi r5, r5, ((offsets - .getpc)-4)@l
    lwzu    r3,  4(r5) # r3  = TEX1.tab 1
    lwzu    r10, 4(r5) # r10 = TEX1.tab 2
    lwz     r3,  0(r3)
    lwz     r10, 0(r10)
    lwzu    r11, 4(r5)  # mask for table 1
    lwzu    r12, 4(r5)  # flags for table 1
    #cmpwi   r3,  0     # not loaded?
    #beq     abort

    li      r9, BASE_ID
    .next:
        lwzu    r6, 4(r5)  # r6 = offset

        # there's a gap in the middle of the IDs.
        # we detect this by it beginning with 0x01
        # instead of 0x8x. we don't want to add an
        # offset to that entry.
        srwi    r7, r6, 24
        cmpwi   r7, 1
        beq     .skip2
            add     r6, r6, r4 # plus the offset we wrote it at

        .noAdd:
        slwi    r7, r9,  2  # ID -> offset
        cmpwi   r3, 0
        beq     .skip1
            # for this table we need it to start with 0x4x, not 0x8x.
            and     r14, r6,  r11
            or      r14, r14, r12
            stwx    r14, r3,  r7 # store it to table 1

        .skip1:
        cmpwi   r10, 0
        beq     .skip2
            stwx    r6, r10, r7 # store it to table 2

        .skip2:
        addi    r9, r9,  1
        cmpwi   r9, LAST_ID
        bne     .next

abort:
    blr

start:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stmw    r3, SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3, SP_LR_SAVE(r1)

    bl      do_patch

    lwz     r3, SP_LR_SAVE(r1)
    mtlr    r3
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    li      r9, 0x1000

# bin2gecko adds the necessary padding but that doesn't work
# if there's multiple patches.
#nop # so we need this here.
GECKO_END_PATCH

# seconary patch for other texture load function,
# used when leaving orbit.
#GECKO_BEGIN_PATCH 0x80042888, tex1_2 # blr
#b do_patch
#GECKO_END_PATCH tex1_2

#GECKO_BEGIN_PATCH 0x800427bc, tex1_3 # blr
#b do_patch
#GECKO_END_PATCH tex1_3
