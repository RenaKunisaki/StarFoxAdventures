# TEX1.bin patch
# Injects Krystal textures into TEX1.bin.

tex1BinPatch:
    # replacing a call to allocTagged.
    # r3=size, r4=tag, r5=name (null)
    # This is pretty much identical to the MODELS.BIN patch.
    # We append Krystal's textures to the end of TEX1.BIN in memory
    # just like we did for the model.

    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stw     r3,  SP_ORIG_SIZE(r1) # original file size
    stw     r4,  SP_ALLOC_TAG(r1) # original alloc tag
    mflr    r6
    stw     r6,  SP_LR_SAVE(r1)

    bl   .tex1Bin_getpc
    .tex1Bin_getpc:
        mflr r14

    LOADWH  r4, CUR_CHAR_ADDR
    LOADBL2 r3, CUR_CHAR_ADDR, r4
    srwi    r3, r3, 7

    cmpwi r3, 0
    beq   .tex1Bin_noCheat
    addi  r3, r14, texturePath2 - .tex1Bin_getpc
    b     .tex1Bin_doLoad

.tex1Bin_noCheat:
    addi  r3, r14, texturePath1 - .tex1Bin_getpc

.tex1Bin_doLoad:
.if DEBUG
    mr      r15, r3
    addi    r3, r14, msg_whichTex1Bin - .tex1Bin_getpc
    mr      r4, r15
    CALL    OSReport
    mr      r3, r15
.endif

    # the rest is the same as the MODELS.BIN patch.
    b doLoadAsset
