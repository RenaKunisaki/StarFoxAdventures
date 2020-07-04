# autosave patch:
# 1) saves the game when moving between maps
# 2) removes "saving" message so that isn't so bothersome
.text
.include "common.s"
.include "globals.s"

# define patches
patchList:
    PATCH_ID "AutoSav" # must be 7 chars
    PATCH_BL 0x80042ec4, onMapLoad
    PATCH_MAIN_LOOP mainLoop
    # disable message
    PATCH_WORD 0x8007db50, 0x60000000
    PATCH_WORD 0x8007db94, 0x60000000
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,0x100 # how much to reserve
    .set SP_LR_SAVE,0x104 # this is what the game does
    .set SP_GPR_SAVE,0x20

entry: # called as soon as our patch is loaded.
    # nothing to do
    blr

mainLoop: # called from main loop. r3 = mainLoop
    mr      r14, r3
    mflr    r16
    LOADW   r3, PATCH_STATE_PTR
    lbz     r4, SAVE_TEXT_COUNTDOWN(r3)
    cmpwi   r4, 0
    beq     .noDrawSaveText

    subi    r4, r4, 1
    stb     r4, SAVE_TEXT_COUNTDOWN(r3)

    # draw "Saving" text for a while, so it can be seen.
    li      r3, 0xFF
    li      r4, 0xFF
    li      r5, 0xFF
    li      r6, 0xFF
    CALL    gameTextSetColor

    addi    r3, r14, s_saving - mainLoop
    li      r4, 0 # box
    li      r5, 0 # x
    li      r6, 0 # y
    CALL    gameTextShowStr

.noDrawSaveText:
    mtlr    r16
    blr

onMapLoad:
    # called when about to load another map.
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r5
    stw     r5, SP_LR_SAVE(r1)
    stmw    r3, SP_GPR_SAVE(r1)

    LOADW   r9,  PATCH_STATE_PTR
    lbz     r4,  AUTOSAVE_ENABLED(r9)
    cmpwi   r4,  0
    beq     .end

    bl      .getpc
    .getpc: mflr r14


    LOADWH  r3, curSaveSlot
    LOADBL2 r4, curSaveSlot, r3
    cmpwi   r4, 0x80
    bge     .end # no save slot
    LOADBL2 r4, 0x803db424, r3 # save status
    cmpwi   r4, 1
    bne     .end # cannot save

    # set text
    li    r4, 60
    stb   r4, SAVE_TEXT_COUNTDOWN(r9)

    # we'll save immediately because we're about to load a new map,
    # which is one of the most likely times to run out of memory
    # and crash, so it would be nice to save before that.
    # the message technically shows up after, but oh well.
    CALL    saveGame_save
    # XXX check for error
.end:
    lwz     r5, SP_LR_SAVE(r1)
    mtlr    r5 # restore LR
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    JUMP    0x800443cc, r0 # mapLoadDataFile

s_saving: .string "Saving..."
