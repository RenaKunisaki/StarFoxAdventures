# autosave patch:
# 1) saves the game when moving between maps
# 2) removes "saving" message so that isn't so bothersome
# NOTE: if you swap memory cards (or load a save state) this patch
# will not show the "not same card" message but also will not save
# and the game will remove the save option from the pause menu.
.text
.include "common.s"

# define patches
patchList:
    PATCH_ID "AutoSav" # must be 7 chars
    PATCH_BL 0x80042ec4, main
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

main:
    # called when about to load another map.
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r5
    stw     r5, SP_LR_SAVE(r1)
    stmw    r3, SP_GPR_SAVE(r1)

    bl      .getpc
    .getpc: mflr r14

    LOADWH  r3, curSaveSlot
    LOADBL2 r4, curSaveSlot, r3
    cmpwi   r4, 0x80
    bge     .end # no save slot
    li      r3, 0xFF
    li      r4, 0xFF
    li      r5, 0xFF
    li      r6, 0xFF
    CALL    gameTextSetColor

    addi    r3, r14, s_saving - .getpc
    li      r4, 0 # box
    li      r5, 0 # x
    li      r6, 0 # y
    CALL    gameTextShowStr
    CALL    0x800e86d0 # save
    # XXX check for error
.end:
    lwz     r5, SP_LR_SAVE(r1)
    mtlr    r5 # restore LR
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    JUMP    0x800443cc, r0 # mapLoadDataFile

s_saving: .string "Saving..."
