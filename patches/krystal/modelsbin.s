# MODELS.bin patch
# Injects Krystal model into every MODELS.bin when it's loaded
# to ensure it's always available.

modelsBinPatch:
    # replacing a call to allocTagged.
    # r3=size, r4=tag, r5=name (null)
    # This is allocating the buffer to read MODELS.BIN into.
    # We'll extend that buffer, and append the Krystal model to it,
    # ensuring that it's available in every map.
    # Later, we'll patch MODELS.TAB to reference it.

    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stw     r3,  SP_ORIG_SIZE(r1) # original file size
    stw     r4,  SP_ALLOC_TAG(r1) # original alloc tag
    mflr    r6
    stw     r6,  SP_LR_SAVE(r1)

    bl .modelsBin_getpc
    .modelsBin_getpc:
        mflr r14

    LOADW   r4, PATCH_STATE_PTR
    lbz     r3, CUR_CHAR_ADDR(r4)
    srwi    r3, r3, 7

.if DEBUG
    mr      r15, r3
    addi    r3, r14, msg_whichModel - .modelsBin_getpc
    mr      r4, r15
    CALL    OSReport
    mr      r3,  r15
.endif

    cmpwi r3, 0
    beq   .modelsBin_noCheat
    addi  r3, r14, modelPath2 - .modelsBin_getpc
    b     doLoadAsset

.modelsBin_noCheat:
    addi  r3, r14, modelPath1 - .modelsBin_getpc

.modelsBin_doLoad:
.if DEBUG
    mr      r15, r3
    addi    r3, r14, msg_loadAsset - .modelsBin_getpc
    mr      r4, r15
    CALL    OSReport
    mr      r3,  r15
.endif

doLoadAsset:
    # load model file and get its size
    addi    r4, r1, SP_EXTRA_SIZE
    CALL    loadFileByPath
    # now r3 = data, sp(SP_EXTRA_SIZE) = size
    stw     r3, SP_FILE_BUFFER(r1)
    cmpwi   r3, 0 # did we load the file?
    beq     loadFailed

    # add the size of the new model, and do the allocation.
    lwz     r6, SP_EXTRA_SIZE(r1)
    lwz     r3, SP_ORIG_SIZE(r1)
    add     r3, r3, r6 # add space for the model
    CALL    allocTagged
    stw     r3, SP_BUFFER(r1)
    cmpwi   r3, 0
    bne     modelsBin_ok

loadFailed:
    # if we get here, either we couldn't load the file, or
    # we didn't have enough free memory for the buffer.
#.if DEBUG
#    addi    r3, r14, msg_loadModelFailed - .modelsBin_getpc
#    CALL    OSReport
#.endif

    lwz     r3, SP_ORIG_SIZE(r1) # try again with orig size
    lwz     r4, SP_ALLOC_TAG(r1)
    li      r5, 0 # name
    CALL    allocTagged
    b       freeFileAndReturnBuffer # regardless of success

modelsBin_ok:
    # we loaded the file OK and allocated the buffer.
    # now copy the file into the buffer.

#.if DEBUG
#    addi    r3, r14, msg_loadModel - .modelsBin_getpc
#    lwz     r4, SP_BUFFER(r1)
#    lwz     r5, SP_EXTRA_SIZE(r1)
#    CALL    OSReport
#.endif

memcpyAndReturnBuffer:
    lwz     r3, SP_BUFFER(r1)      # r3 = buf
    lwz     r4, SP_FILE_BUFFER(r1) # r4 = src
    lwz     r5, SP_EXTRA_SIZE(r1)  # r5 = size
    lwz     r0, SP_ORIG_SIZE(r1)   # r0 = original size of buffer
    add     r3, r3, r0             # r3 = dest
    CALL    memcpy

freeFileAndReturnBuffer:
    # free the file buffer
    lwz     r3, SP_FILE_BUFFER(r1)
    cmpwi   r3, 0
    beq     returnBuffer
    CALL    mm_free # free our loaded file, if any

returnBuffer:
    # we hooked a call to allocTagged, so we need to return
    # the address of the buffer we just allocated.
    lwz     r3, SP_BUFFER(r1)
    lwz     r6, SP_LR_SAVE(r1)
    mtlr    r6
    addi    r1, r1, STACK_SIZE # restore stack ptr
    blr
