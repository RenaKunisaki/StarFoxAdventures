.text
.include "common.s"

# Patch TEX1.tab loading; set Krystal texture offsets we loaded in patch 3
GECKO_BEGIN_PATCH 0x800463DC, tex1_1 # lwz r3, -0x6554(r13)
# just before a call to stackPush
# r3, r4 are free
# nearly identical to patch 2
b start

.set MAP_TEX1_TAB,0x8035f518
.set GLOBAL_TEX1_TAB,0x8035f46c
.set NUM_TEXTURES,7
.set BASE_OFFSET,0x9EB90 # 0x13D720 >> 1 - offset we copy from in TEX1.bin
.set BASE_ID,0x724 # first ID to patch in table
.set LAST_ID,BASE_ID+NUM_TEXTURES

offsets:
    .int MAP_TEX1_TAB, GLOBAL_TEX1_TAB

textureData:
    # copied from TEX1.tab and offset by the first entry.
    .int 0x8409EB90 - BASE_OFFSET
    .int 0x810A1940 - BASE_OFFSET
    .int 0x810AA910 - BASE_OFFSET
    .int 0x01000000
    .int 0x810AB270 - BASE_OFFSET
    .int 0x810AB600 - BASE_OFFSET
    .int 0x810AB740 - BASE_OFFSET

# using .align generates unnecessary extra padding.
#.byte 0, 0
do_patch:
    # get the texture offset, stored by previous code.
    lis     r4, 0x8180
    lwz     r4, -8(r4)
    srwi    r4, r4, 1 # divide by 2 as the game expects

    # get table address in r5
    mflr r7
    bl   .getpc
    .getpc:
        mflr r5
        mtlr r7 # restore LR
        # subtract 4 so we can use lwzu
        addi r5, r5, ((offsets - .getpc)-4)@l
    lwzu    r3,  4(r5) # r3  = map TEX1.tab
    lwzu    r9,  4(r5) # r9  = global TEX1.tab
    lwz     r3,  0(r3)
    cmpwi   r3,  0     # map table not loaded?
    bne     .ok
    lwz     r3,  0(r9) # get the global one

    .ok:
    li      r9, BASE_ID
    .next:
        lwzu    r6, 4(r5)  # r6 = offset
        srwi    r7, r6, 24
        cmpwi   r7, 1
        beq     .noAdd

        add     r6, r6, r4 # plus the offset we wrote it at

        .noAdd:
        slwi    r7, r9,  2  # ID -> offset
        stwx    r6, r3,  r7 # store it
        addi    r9, r9,  1

        cmpwi   r9, LAST_ID
        bne     .next
    blr

start:
    mflr    r10
    bl      do_patch
    mtlr    r10
    lwz     r3, -0x6554(r13) # replaced

# bin2gecko adds the necessary padding but that doesn't work
# if there's multiple patches.
nop # so we need this here.
GECKO_END_PATCH tex1_1

# seconary patch for other texture load function,
# used when leaving orbit.
GECKO_BEGIN_PATCH 0x80042888, tex1_2 # blr
b do_patch
GECKO_END_PATCH tex1_2

GECKO_BEGIN_PATCH 0x800427bc, tex1_3 # blr
b do_patch
GECKO_END_PATCH tex1_3
