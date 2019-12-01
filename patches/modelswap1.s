.text
.include "common.s"

# Patch MODELS.bin loading; inject Krystal model into it
GECKO_BEGIN_PATCH 0x800453D4 # lis r4, 0x7d7d
# just before a call to allocTagged; r3 = size
# r5 is free
b start

.set KRYSTAL_MODEL_OFFSET,0x901C0
.set KRYSTAL_MODEL_SIZE,0x12B40
# stack offsets
.set STACK_SIZE,0x40 # how much to reserve
.set SP_BUFFER,0x10 # allocated buffer
.set SP_MODEL_SIZE,0x14 # KRYSTAL_MODEL_SIZE
.set SP_ORIG_SIZE,0x18 # original buffer size (offset to copy to)
.set SP_FILE_BUFFER,0x1C # file buffer temp

filePath:
    .string "warlock/MODELS.bin"

.byte 0 # align without excess padding
start:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stw     r3,  SP_ORIG_SIZE(r1)
    # store the offset, we'll need it later.
    lis     r4, 0x8180
    stw     r3, -4(r4) # hopefully this is safe...

    # call allocTagged ourselves; allocate the requested size
    # plus the size of the model data
    LOAD    r5, KRYSTAL_MODEL_SIZE
    stw     r5, SP_MODEL_SIZE(r1)
    add     r3, r3, r5 # add space for the model
    li      r5, 0
    LOAD    r4, 0x7d7d7d7d # alloc tag
    CALL    allocTagged
    # now r3 = buffer
    stw     r3, SP_BUFFER(r1)
    cmpwi   r3, 0
    #bne     .ok
    beq     end # alloc fail is OK
    #CALL    0x80248fd0 # a random OSPanic() we can recognize

.ok:
    # get file path address in r3
    #mflr r7
    bl   .getpc
    .getpc:
        mflr r3
        #mtlr r7 # restore LR
        addi r3, r3, (filePath - .getpc)@l
    # load "warlock/MODELS.bin"
    li      r4, 0 # we don't need size
    CALL    loadFileByPath
    # now r3 = data
    stw     r3, SP_FILE_BUFFER(r1)

    # copy from there into buffer
    LOAD    r0, KRYSTAL_MODEL_OFFSET
    add     r4, r3, r0     # r4 = src
    lwz     r3, SP_BUFFER(r1)     # r3 = buffer
    lwz     r5, SP_MODEL_SIZE(r1) # r5 = size
    lwz     r0, SP_ORIG_SIZE(r1)  # r0 = original size of buffer
    add     r3, r3, r0     # r3 = dest
    CALL    memcpy         # returns dest

    # free the file buffer
    lwz     r3, SP_FILE_BUFFER(r1)
    CALL    free

end:
    lwz     r3, SP_BUFFER(r1)  # return buffer
    addi    r1, r1, STACK_SIZE # restore stack ptr

    # jump back into the game code, just past the allocTagged call.
    # we can't use a branch here because we don't know our location,
    # and `ba` isn't usable on GCN at all (unless someone mapped RAM
    # at 0)
    JUMP    0x800453E4, r4

GECKO_END_PATCH
