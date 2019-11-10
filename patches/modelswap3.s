.text
.include "common.s"

# Patch TEX0.bin loading; inject Krystal textures into it
GECKO_BEGIN_PATCH 0x80045CC0 # lis r4, 0x7d7d
# just before a call to allocTagged; r3 = size
# r5 is free
# yes, this is nearly the same as patch 1
b start

.set NUM_TEXTURES,4
.set TEXTURE_OFFSETS_SIZE,(NUM_TEXTURES+1)*4
# +2 bytes for the end marker and 2 for the lhzu padding
.set TOTAL_EXTRA_SIZE,0x540 # how much to add to the buffer

# stack offsets
.set STACK_SIZE,0x40 # how much to reserve
.set SP_BUFFER,0x10 # allocated buffer
.set SP_DATA_SIZE,0x14 # TOTAL_EXTRA_SIZE
.set SP_ORIG_SIZE,0x18 # original buffer size (offset to copy to)
.set SP_FILE_BUFFER,0x1C # file buffer temp
.set SP_DEST_BUFFER,0x20 # destination in allocated buffer
.set SP_TEXOFFSETS,0x24 # ptr to textureOffsets

textureOffsets: # offset >> 4, data size, for textures 0xD4-D9
    # we don't need the actual IDs here though
    # we'll round sizes up to next 16; the exta data won't matter
    # but they do need to be aligned.
    .short 0x2ED2, 0x00E0 # 0x009F + 0x40 rounded up
    .short 0x2F8A, 0x00D0 # 0x00AB + 0x24
    .short 0x2FF2, 0x0090 # 0x006B + 0x24
    .short 0x3040, 0x0300 # 0x02BC + 0x40
    .short 0

filePath:
    .string "warlock/TEX0.bin"

.byte 0 # align without excess padding
start:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stw     r3,  SP_ORIG_SIZE(r1)
    # store the offset, we'll need it later.
    lis     r4, 0x8180
    stw     r3, -8(r4)

    # call allocTagged ourselves; allocate the requested size
    # plus the size of the model data
    li      r5, TOTAL_EXTRA_SIZE
    stw     r5, SP_DATA_SIZE(r1)
    add     r3, r3, r5 # add space for the new data
    li      r5, 0
    lis     r4, 0x7d7d
    ori     r4, r4, 0x7d7d
    CALL    allocTagged
    # now r3 = buffer
    stw     r3, SP_BUFFER(r1)
    addi    r3, r3, TOTAL_EXTRA_SIZE
    stw     r3, SP_DEST_BUFFER(r1)

    # get table address in r3
    #mflr r7
    bl   .getpc
    .getpc:
        mflr r3
        #mtlr r7 # restore LR
        # subtract 2 so we can use lhzu
        addi r3, r3, ((textureOffsets - .getpc)-2)@l
    stw     r3, SP_TEXOFFSETS(r1)
    addi    r3, r3, TEXTURE_OFFSETS_SIZE

    # load "warlock/TEX0.bin"
    li      r4, 0 # we don't need size
    CALL    loadFileByPath
    # now r3 = data
    stw     r3, SP_FILE_BUFFER(r1)

    # copy the texture data we want.
    .next:
        # get texture ID
        lwz     r6, SP_TEXOFFSETS(r1)
        lhzu    r4, 2(r6) # r4 = offset >> 4
        cmpwi   r4, 0
        beq     end

        slwi    r4, r4, 4 # r4 = offset
        lhzu    r5, 2(r6) # r5 = length
        stw     r6, SP_TEXOFFSETS(r1) # store new offset ptr

        lwz     r3, SP_FILE_BUFFER(r1)
        add     r4, r4, r3 # r4 = src
        lwz     r3, SP_DEST_BUFFER(r1)
        add     r6, r3, r5 # r6 is ignored by memcpy
        stw     r6, SP_DEST_BUFFER(r1) # update ptr

        CALL    memcpy
        b       .next

end:
    # free the file buffer
    lwz     r3, SP_FILE_BUFFER(r1)
    CALL    free
    lwz     r3, SP_BUFFER(r1)  # return buffer
    addi    r1, r1, STACK_SIZE # restore stack ptr

    # jump back into the game code, just past the allocTagged call.
    # we can't use a branch here because we don't know our location,
    # and `ba` isn't usable on GCN at all (unless someone mapped RAM
    # at 0)
    lis     r4, 0x8004
    ori     r4, r4, 0x5CD0
    mtlr    r4
    blr

GECKO_END_PATCH
