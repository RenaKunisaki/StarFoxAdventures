# Krystal mod v2.0
# Implemented as a patch file instead of a Gecko code.
# Improvements:
# - The SharpClaw Disguise now functions properly.
# - Switching character now requires holding L and pressing D-pad Down,
#   instead of just D-pad Down. So you can zoom the map out without
#   switching. Also, there's now a sound effect.
# - Reduce load time and RAM usage by loading the assets from dedicated
#   files instead of animtest, and by removing a lot of repeated code.
# TODO:
# - Patch animations instead of using Fox's.
.text
.include "common.s"

constants:
    # controls
    .set CHAR_SWAP_BUTTONS_PRESS,PAD_BUTTON_DOWN # must press this...
    .set CHAR_SWAP_BUTTONS_HOLD,PAD_BUTTON_L # while holding these.

    # model data
    .set KRYSTAL_MODEL_ID,0x4E8

    # texture data
    .set NUM_TEXTURES,7 # how many entries in tex1Tab_data
    .set TEX1_BASE_OFFSET,0x023430 # first entry
    .set TEX1_BASE_ID,0x724 # first ID to patch in table
    .set TEX1_LAST_ID,TEX1_BASE_ID+NUM_TEXTURES

    # stack for all patches
    # since gnu `as` has no concept of local labels, these are just
    # shared among all of them. (XXX can we re-set?)
    .set STACK_SIZE,    0x100 # how much to reserve
    .set SP_LR_SAVE,    0x104 # this is what the game does
    .set SP_GPR_SAVE,   0x10  # for modelsTabPatch and tex1TabPatch

    # for modelsBinPatch and tex1BinPatch
    .set SP_BUFFER,     0x10 # allocated buffer
    .set SP_EXTRA_SIZE, 0x14 # size of our file
    .set SP_ORIG_SIZE,  0x18 # original buffer size (offset to copy to)
    .set SP_FILE_BUFFER,0x1C # file buffer temp
    .set SP_ALLOC_TAG,  0x20 # original alloc tag

    # for tex1BinPatch
    .set SP_DEST_BUFFER,0x24 # destination in allocated buffer
    .set SP_SRC_OFFSET, 0x28 # SOURCE_OFFSET

# define patches
patchList:
    PATCH_ID        "Krystal" # must be 7 chars
    PATCH_BL        0x800453E0, modelsBinPatch
    PATCH_B         0x80043D7C, modelsTabPatch
    PATCH_BL        0x80046164, tex1BinPatch
    PATCH_B         0x80043DB8, tex1TabPatch
    PATCH_B         0x802B6F28, loadSaveGamePatch
    PATCH_MAIN_LOOP mainLoopPatch
    PATCH_WORD      0x802B08D8, 0x38000001 # let Krystal use staff
    PATCH_WORD      0x80295BE0, 0x38600001 # let Krystal use map, PDA, etc
    PATCH_WORD      0x8005F5E0, 0x60000000 # HACK: fix a crash caused by
        # the previous patch, where some fog parameter is being
        # set incorrectly leading to an endless loop on pause screen.
        # XXX figure out what's causing that and fix it correctly.
        # this workaround causes some flickering.
    PATCH_END PATCH_KEEP_AFTER_RUN

entry: # called when patch is loaded
    blr # nothing to do

########################## MODELS.BIN PATCH ##############################

modelsBinPatch:
    # replacing a call to allocTagged.
    # r3=size, r4=tag, r5=name (null)
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stw     r3,  SP_ORIG_SIZE(r1) # original file size
    stw     r4,  SP_ALLOC_TAG(r1) # original alloc tag
    mflr    r6
    stw     r6,  SP_LR_SAVE(r1)

    # get file path address in r3
    bl .modelsBin_getpc
    .modelsBin_getpc:
        mflr r14
        addi r3, r14, (modelPath - .modelsBin_getpc)@l

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
    #addi    r3, r14, (msg_loadModelFailed - .modelsBin_getpc)@l
    #CALL    OSReport

    lwz     r3, SP_ORIG_SIZE(r1) # try again with orig size
    lwz     r4, SP_ALLOC_TAG(r1)
    li      r5, 0 # name
    CALL    allocTagged
    b       freeFileAndReturnBuffer # regardless of success

modelsBin_ok:
    # we loaded the file OK and allocated the buffer.
    # now copy the file into the buffer.

    #addi    r3, r14, (msg_loadModel - .modelsBin_getpc)@l
    #lwz     r4, SP_BUFFER(r1)
    #lwz     r5, SP_EXTRA_SIZE(r1)
    #CALL    OSReport

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
    CALL    mm_free

returnBuffer:
    lwz     r3, SP_BUFFER(r1)  # return buffer

abort:
    lwz     r6,  SP_LR_SAVE(r1)
    mtlr    r6
    addi    r1, r1, STACK_SIZE # restore stack ptr
    blr


#################### TEX1.BIN PATCH ##############################

tex1BinPatch:
    # replacing a call to allocTagged.
    # r3=size, r4=tag, r5=name (null)
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stw     r3,  SP_ORIG_SIZE(r1) # original file size
    stw     r4,  SP_ALLOC_TAG(r1) # original alloc tag
    mflr    r6
    stw     r6,  SP_LR_SAVE(r1)

    # get file path address in r3
    bl   .tex1Bin_getpc
    .tex1Bin_getpc:
        mflr r14
    addi r3, r14, (texturePath - .tex1Bin_getpc)@l

    # the rest is the same as the MODELS.BIN patch.
    b doLoadAsset


####################### MODELS.TAB PATCH ##########################

modelsTab_doPatch:
    # subroutine called from modelsTabPatch
    # to apply the patch to the file slots
    # r3 = models.bin file ID * 4
    # r4 = models.tab file ID * 4
    lwzx    r9,  r3, r11 # r9 = &MODELS.bin
    cmpwi   r9,  0
    beq     .modelsTab_doPatch_end # not loaded, skip

    # get the size of this file
    lwzx    r5, r3, r12 # r5 = size
    # that's the size of the original file, before we appended
    # our model data, so we don't need to adjust it.
    add     r7, r5, r9 # r7 = the actual address
    lwz     r7, 0(r7)
    cmp     0, 0, r7, r10 # is there a model here?
    # we don't need to check for other valid headers because we're
    # only checking for a specific model
    bne     .modelsTab_doPatch_end

    # OK, r5 is valid.
    lis     r9, 0x8000 # flags: compressed
    or      r5, r5, r9
    lwzx    r9, r4, r11 # r9 = MODELS.tab
    cmpwi   r9, 0
    beq     .modelsTab_doPatch_end # table not loaded!? idk why this happens.
    stw     r5, (KRYSTAL_MODEL_ID*4)(r9) # store the offset

    .modelsTab_doPatch_end:
        blr

modelsTabPatch:
    # patch mergeTableFiles() case MODELS.tab
    # insert the offset we loaded Krystal at into the table
    # patch at 0x80043d7c, "b 0x80043df4" -> "b modelsTabPatch"
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stmw    r3, SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3, SP_LR_SAVE(r1)

    LOAD    r12, dataFileSize
    addi    r11, r12, dataFileBuffers - dataFileSize
    LOAD    r10, 0xFACEFEED

    # patch main slot
    li      r3, MODELS_BIN*4
    li      r4, MODELS_TAB*4
    bl      modelsTab_doPatch

    # patch secondary slot
    li      r3, MODELS_BIN2*4
    li      r4, MODELS_TAB2*4
    bl      modelsTab_doPatch

    # restore registers
endTabPatch:
    lwz     r3, SP_LR_SAVE(r1)
    mtlr    r3
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr

    # jump back to game code
    # the instruction we replaced was also a branch to here.
    JUMP    0x80043df4, r11


######################### TEX1.TAB PATCH ########################

tex1_doPatch:
    # r3 = tex1.bin file ID * 4
    # r4 = tex1.tab file ID * 4
    lwzx    r9,  r3, r11 # r9 = &TEX1.bin
    cmpwi   r9,  0
    beq     .tex1_doPatch_end # not loaded, skip

    # get the size of this file
    lwzx    r5, r3, r12 # r5 = size
    # that's the size of the original file, before we appended
    # our texture data, so we don't need to adjust it.
    addi    r5, r5, 0x20 # except this for some reason

    add     r7, r5, r9 # r7 = the actual address
    lwz     r7, 0(r7)
    cmpwi   r7, 0x14 # is our texture here?
    bne     .tex1_doPatch_end

    # OK, r5 is valid.

    srwi    r5, r5, 1 # divide by 2 as the game expects
    lwzx    r9, r4, r11 # r9 = TEX1.tab
    cmpwi   r9, 0
    beq     .tex1_doPatch_end # table not loaded!? idk why this happens.

    li      r3, TEX1_BASE_ID
    mr      r4, r10
.tex1_doPatch_next:
    lwzu    r6, 4(r4)  # r6 = offset

    # there's a gap in the middle of the IDs.
    # we detect this by it beginning with 0x01
    # instead of 0x8x. we don't want to change
    # that entry at all.
    srwi    r7, r6, 24
    cmpwi   r7, 1
    beq     .tex1_doPatch_skip

    # add the offset we loaded the texture to,
    # and the offset from our table below.
    add     r6, r6, r5 # plus the offset we wrote it at
    slwi    r7, r3,  2 # ID -> offset
    stwx    r6, r7, r9 # store the offset

.tex1_doPatch_skip:
    addi    r3, r3,  1
    cmpwi   r3, TEX1_LAST_ID
    bne     .tex1_doPatch_next

.tex1_doPatch_end:
    blr


tex1TabPatch:
    # patch mergeTableFiles() case TEX1.tab
    # insert the offset we loaded Krystal at into the table
    # nearly identical to modelsTabPatch
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stmw    r3, SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3, SP_LR_SAVE(r1)

    # get table address in r5
    bl   .tex1Tab_getpc
    .tex1Tab_getpc:
        mflr r10
        # subtract 4 so we can use lwzu
        addi r10, r10, ((tex1Tab_offsets - .tex1Tab_getpc)-4)@l

    lwzu    r12, 4(r10) # r12 = dataFileSize
    addi    r11, r12, dataFileBuffers - dataFileSize

    li      r3, TEX1_BIN*4
    li      r4, TEX1_TAB*4
    bl      tex1_doPatch

    li      r3, TEX1_BIN2*4
    li      r4, TEX1_TAB2*4
    bl      tex1_doPatch
    b       endTabPatch # reuse code


##################### LOAD SAVEGAME PATCH #######################

loadSaveGamePatch:
    # Patch the very end of objLoadPlayerFromSave
    # by patching here we can reuse the function's own stack
    # r28 = ObjInstance*
    lwz     r3, 0x7C(r28) # get models ptr
    lwz     r4, 0x00(r3)  # get Krystal model
    lwz     r5, 0x04(r3)  # get Fox model
    lwz     r6, 0x00(r4)  # Krystal header
    lwz     r7, 0x00(r5)  # Fox header
    lwz     r0, 0x6C(r7)  # Fox anim ptr
    stw     r0, 0x6C(r6)  # store to Krystal

    # switch to Krystal immediately.
    # otherwise, she'll be in a T pose when loading the game.
    # this is because the main loop patch won't switch the model
    # until one frame after the animations are initialized.
    # this didn't happen with the Gecko code, because it executes
    # every frame no matter what.
    # by doing this we ensure that the animations initialize for
    # Krystal, since we'll be swapping to her immediately anyway.
    # the swap happens before the screen fades in, so it's still
    # unnoticeable, and now she'll stand normally.
    li      r4, 0
    stb     r4, 0xAD(r28)

    # copy Fox's animIdxs to Krystal so the animations will play correctly
    # as well as field 0x80 which is right after
    li      r4, 0x70
    .loadSaveGamePatch_next:
        lwzx    r0,r4,r7 # get Fox animIdxs
        stwx    r0,r4,r6 # store to Krystal
        addi    r4,r4,4
        cmpwi   r4,0x84
        bne     .loadSaveGamePatch_next

    li   r0, 0 # replaced
    JUMP 0x802B6F2C, r11


###################### MAIN LOOP PATCH ##########################

mainLoopPatch:
    # r3 = address of mainLoopPatch
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stmw    r3, SP_GPR_SAVE(r1)
    mflr    r15
    stw     r15, SP_LR_SAVE(r1)
    mr      r10, r3
    lbz     r15, (curCharacter - mainLoopPatch)(r10)

    # check buttons
    LOADWH  r9, controllerStates
    LOADWL2 r7, buttonsJustPressed, r9 # buttons pressed
    LOADHL2 r8, controllerStates,   r9 # buttons held
    andi.   r3, r8, CHAR_SWAP_BUTTONS_HOLD
    beq     .mainLoopPatch_noCharSwap
    andi.   r3, r7, CHAR_SWAP_BUTTONS_PRESS
    beq     .mainLoopPatch_noCharSwap

    # swap character
    xori  r15, r15, 1
    stb   r15, (curCharacter - mainLoopPatch)(r10)

    # do sharpclaw disguise animation (XXX where is that function?)
    #LOADW r3, pPlayer
    #li    r4, 9
    #CALL 0x800395d8

    # play sound effect
    li    r3, 0
    li    r4, 0x402
    CALL  audioPlaySound

.mainLoopPatch_noCharSwap:
    LOADW r3, pPlayer
    cmpwi r3, 0
    beq   .mainLoopPatch_end

    # ensure the player is object ID 1.
    # on the title screen, the Fox object is the player,
    # but it's not actually the same object type as the playable
    # Fox, and trying to change its model ID will crash the game.
    # it's 0x30
    lhz   r4, 0x44(r3)
    cmpwi r4, 1
    bne   .mainLoopPatch_end

    # don't apply when using SharpClaw disguise
    lbz   r4, 0xAD(r3)
    cmpwi r4, 2
    beq   .mainLoopPatch_end

    # swap character
    stb   r15, 0xAD(r3) # model index
    lwz   r4,  0xB8(r3)
    stb   r15, 0x081B(r4) # voice and backpack

.mainLoopPatch_end:
    lwz     r3, SP_LR_SAVE(r1)
    mtlr    r3
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    blr

#################### STRINGS AND CONSTANT POOLS ###################


tex1Tab_offsets: # for quick access
    .int dataFileSize

tex1Tab_data:
    # copied from TEX1.tab and offset by the first entry.
    #.int 0x84023430 - TEX1_BASE_OFFSET # eye texture
    #.int 0x810261E0 - TEX1_BASE_OFFSET # fur texture
    #.int 0x8102F1B0 - TEX1_BASE_OFFSET # ? maybe hair sides?
    #.int 0x01000000
    #.int 0x8102FB10 - TEX1_BASE_OFFSET # another eye texture?
    #.int 0x8102FEA0 - TEX1_BASE_OFFSET # hair front
    #.int 0x8102FFE0 - TEX1_BASE_OFFSET # hair back

    # for alt
    .int  0x84000000
    .int  0x81002DB0
    .int  0x81012308
    .int  0x01000000
    .int  0x81012C68
    .int  0x81012FF8
    .int  0x81013138

# let's make the paths short to shave a few more bytes
# off the patch size, since RAM is tight.
modelPath:
    .string "patches/km"

texturePath:
    .string "patches/kt"

#msg_loadModel:
#    .string "Load model to offs %08X sz %08X"
#msg_loadModelFailed:
#    .string "Load model FAIL"

curCharacter:
    .byte 0 # keeps track of whether to apply swap.
