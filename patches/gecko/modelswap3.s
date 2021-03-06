.text
.include "common.s"

# Patch TEX1.bin loading; inject Krystal textures into it
GECKO_BEGIN_PATCH 0x80046158 # lis r4, 0x7d7d
# just before a call to allocTagged; r3 = size
# r5 is free
# r23 = which file is being loaded: 0x20 (main map) or 0x4B (submap)
# yes, this is nearly the same as patch 1
b start

.set TOTAL_EXTRA_SIZE,0x19a00 # how much to add to the buffer
.set SOURCE_OFFSET,0x046860   # where to copy from

# stack offsets
.set STACK_SIZE,0x40 # how much to reserve
.set SP_BUFFER,0x10 # allocated buffer
.set SP_EXTRA_SIZE,0x14 # TOTAL_EXTRA_SIZE
.set SP_ORIG_SIZE,0x18 # original buffer size (offset to copy to)
.set SP_FILE_BUFFER,0x1C # file buffer temp
.set SP_DEST_BUFFER,0x20 # destination in allocated buffer
.set SP_TEXOFFSETS,0x24 # ptr to textureOffsets (not used atm)
.set SP_SRC_OFFSET,0x28 # SOURCE_OFFSET
.set SP_ALLOC_TAG,0x2C

srcOffset:
    # keep this here, we can retrieve it easily
    .int SOURCE_OFFSET

filePath:
    .string "animtest/TEX1.bin"

.byte 0, 0 # align without excess padding
start:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stw     r3,  SP_ORIG_SIZE(r1)

    # call allocTagged ourselves; allocate the requested size
    # plus the size of the model data
    LOAD    r5, TOTAL_EXTRA_SIZE
    stw     r5, SP_EXTRA_SIZE(r1)
    add     r3, r3, r5 # add space for the new data
    li      r5, 0 # name
    LOAD    r4, 0x7d7d7d7d # the alloc tag
    stw     r4, SP_ALLOC_TAG(r1)
    CALL    allocTagged
    # now r3 = buffer
    cmpwi   r3, 0
    bne     .alloc_ok

    # alloc failed
    lwz     r3, SP_ORIG_SIZE(r1) # try again with original size
    lwz     r4, SP_ALLOC_TAG(r1)
    li      r5, 0 # name
    CALL    allocTagged
    b       abort # return the original result

.alloc_ok:
    stw     r3, SP_BUFFER(r1)
    lwz     r5, SP_ORIG_SIZE(r1)
    add     r3, r3, r5
    stw     r3, SP_DEST_BUFFER(r1)

    # get table address in r3
    #mflr r7
    bl   .getpc
    .getpc:
        mflr r3
        #mtlr r7 # restore LR
        addi r3, r3, (srcOffset - .getpc)@l
    # unfortunately we can't use lwzu here because we'd
    # still have to add 4 to r3 to point it to the file path,
    # which would defeat the point.
    lwz     r4, 0(r3)
    stw     r4, SP_SRC_OFFSET(r1)
    addi    r3, r3, 4

    # load "warlock/TEX0.bin"
    li      r4, 0 # we don't need size
    CALL    loadFileByPath
    # now r3 = data
    stw     r3, SP_FILE_BUFFER(r1)
    cmpwi   r3, 0
    beq     abort
    #CALL    0x80249000 # a random OSPanic() we can recognize

    # copy the texture data
    lwz     r4, SP_SRC_OFFSET(r1)
    add     r4, r4, r3 # r4 = src
    lwz     r3, SP_DEST_BUFFER(r1) # r3 = dest
    lwz     r5, SP_EXTRA_SIZE(r1) # r5 = length
    CALL    memcpy

end:
    # free the file buffer
    lwz     r3, SP_FILE_BUFFER(r1)
    CALL    mm_free
    lwz     r3, SP_BUFFER(r1)  # return buffer

abort:
    addi    r1, r1, STACK_SIZE # restore stack ptr

    # jump back into the game code, just past the allocTagged call.
    # we can't use a branch here because we don't know our location,
    # and `ba` isn't usable on GCN at all (unless someone mapped RAM
    # at 0)
    JUMP    0x80046168, r4

skip:
GECKO_END_PATCH
