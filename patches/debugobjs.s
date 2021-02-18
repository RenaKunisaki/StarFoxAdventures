# debug objects patch:
# allows viewing debug objects, toggled by PDA menu.
# unfortunately this doesn't really work, as turning it on during
# gameplay will crash if such an object is already loaded.
# maybe we need to make them always "visible" and find some other
# way to hide them when this is turned off.
.text
.include "common.s"
.include "globals.s"

# define patches
patchList:
    PATCH_ID "DbgObjs" # must be 7 chars
    PATCH_BL 0x8002d820, main
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,0x100 # how much to reserve
    .set SP_LR_SAVE,0x104 # this is what the game does
    .set SP_X,0x14
    .set SP_Y,0x18
    .set SP_Z,0x1C
    .set SP_GPR_SAVE,0x20

entry: # called as soon as our patch is loaded.
    # nothing to do
    blr

main: # called by our hook from loadCharacter()
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r5
    stw     r5, SP_LR_SAVE(r1)
    stmw    r3, SP_GPR_SAVE(r1)

    LOADW   r3, PATCH_STATE_PTR
    lbz     r4, SHOW_DEBUG_OBJS(r3)
    cmpwi   r4, 0
    beq     .off

    rlwinm	r0, r4, 0, 0, 30
    stw	    r0, 0x0044 (r30)
    lwz     r5, SP_LR_SAVE(r1)
    mtlr    r5 # restore LR
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    JUMP    0x8002d830, r22

.off:
    lwz     r5, SP_LR_SAVE(r1)
    mtlr    r5 # restore LR
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    rlwinm  r0,r4,0x0,0x1f,0x1f # replaced
    JUMP    0x8002d824, r22
.align 4
