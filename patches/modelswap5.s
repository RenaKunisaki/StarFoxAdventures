.text
.include "common.s"

# patch ANIM.BIN loading; inject Krystal animations into it
GECKO_BEGIN_PATCH 0x80045858 # lis r4, 0x7d7d
# just before a call to allocTagged; r3 = size
# r5 is free
# yes, this is nearly the same as patch 1 and 3
b start

.set TOTAL_EXTRA_SIZE,0xC2FA0 # how much to add to the buffer
.set SOURCE_OFFSET,0          # where to copy from

# stack offsets
.set STACK_SIZE,0x40 # how much to reserve
.set SP_BUFFER,0x10 # allocated buffer
.set SP_EXTRA_SIZE,0x14 # TOTAL_EXTRA_SIZE
.set SP_ORIG_SIZE,0x18 # original buffer size (offset to copy to)
.set SP_FILE_BUFFER,0x1C # file buffer temp
.set SP_DEST_BUFFER,0x20 # destination in allocated buffer
.set SP_SRC_OFFSET,0x24 # SOURCE_OFFSET

filePath:
    .string "animtest/ANIM.BIN"

# using .align generates unnecessary extra padding.
.byte 0, 0
start:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stw     r3,  SP_ORIG_SIZE(r1)
    # store the offset, we'll need it later.
    lis     r4, 0x8180
    stw     r3, -0x0C(r4)

    # call allocTagged ourselves; allocate the requested size
    # plus the size of the model data
    lis     r5, TOTAL_EXTRA_SIZE@h
    ori     r5, r5, TOTAL_EXTRA_SIZE@l
    stw     r5, SP_EXTRA_SIZE(r1)
    add     r3, r3, r5 # add space for the new data
    li      r5, 0 # name
    lis     r4, 0x7d7d # the alloc tag, which the game was about
    ori     r4, r4, 0x7d7d # to set up before we hooked it
    CALL    allocTagged
    # now r3 = buffer
    stw     r3, SP_BUFFER(r1)
    lwz     r5, SP_ORIG_SIZE(r1)
    add     r3, r3, r5
    stw     r3, SP_DEST_BUFFER(r1)

    # get file path in r3
    #mflr r7
    bl   .getpc
    .getpc:
        mflr r3
        #mtlr r7 # restore LR
        addi r3, r3, (filePath - .getpc)@l

    # load "warlock/ANIM.bin"
    li      r4, 0 # we don't need size
    CALL    loadFileByPath
    # now r3 = data
    stw     r3, SP_FILE_BUFFER(r1)

    # copy the texture data
    # source offset is 0 so we don't need to add to src
    mr      r4, r3
    lwz     r3, SP_DEST_BUFFER(r1) # r3 = dest
    lwz     r5, SP_EXTRA_SIZE(r1) # r5 = length
    CALL    memcpy

end:
    # free the file buffer
    lwz     r3, SP_FILE_BUFFER(r1)
    CALL    free
    lwz     r3, SP_BUFFER(r1)  # return buffer
    addi    r1, r1, STACK_SIZE # restore stack ptr

    # jump back into the game code, just past the allocTagged call.
    JUMP    0x80045864, r4

GECKO_END_PATCH
