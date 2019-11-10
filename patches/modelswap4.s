.text
.include "common.s"

# Patch TEX0.tab loading; set Krystal texture offsets we loaded in patch 3
GECKO_BEGIN_PATCH 0x80045F48 # lwz r3, -0x6554(r13)
# just before a call to stackPush
# r3, r4 are free
# nearly identical to patch 2
b start

.set TEX0_TAB,0x8035f478
.set NUM_TEXTURES,4

textureData:
    # ID, offset high byte, size >> 4
    .byte 0xD4, 0x8F, 0x0E
    #.byte 0xD5, 0x01, 0x00
    #.byte 0xD6, 0x01, 0x00
    .byte 0xD7, 0x88, 0x0D
    .byte 0xD8, 0x88, 0x09
    .byte 0xD9, 0x8F, 0x30
    # we could leave off the last size to save a byte,
    # but we need to align to 4.
    # using .align generates unnecessary extra padding.

#.byte 0, 0
start:
    # r3 = address of TEX0.tab
    lis     r3, TEX0_TAB@h
    ori     r3, r3, TEX0_TAB@l
    lwz     r3, 0(r3)

    # get the texture offset, stored by previous code.
    lis     r4, 0x8180
    lwz     r4, -8(r4)

    # get table address in r5
    mflr r7
    bl   .getpc
    .getpc:
        mflr r5
        mtlr r7 # restore LR
        # subtract 1 so we can use lbzu
        addi r5, r5, ((textureData - .getpc)-1)@l

    li      r8, 0
    .next:
        lbzu    r6, 1(r5)  # r6 = ID
        slwi    r7, r6, 2  # r7 = TEX0.tab offset
        #add     r7, r6, r3 # r7 = TEX0.tab entry to write to

        lbzu    r6, 1(r5)  # r6 = offset high byte
        slwi    r6, r6, 24 # r6 = xx000000
        or      r6, r6, r4 # r6 = offset
        #stw     r6, 0(r7)  # store offset
        stwx    r6, r3, r7

        lbzu    r6, 1(r5)  # r6 = size >> 4
        slwi    r6, r6, 4  # r6 = size
        add     r4, r4, r6 # next offset

        addi    r8, r8, 1
        cmpwi   r8, NUM_TEXTURES
        bne     .next

lwz     r3, -0x6554(r13) # replaced

GECKO_END_PATCH
