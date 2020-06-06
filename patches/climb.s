# climb patch:
# 1) lets you press A to drop off a wall
# 2) increases climbing speed
.text
.include "common.s"

# climbing is animations (alternating):
# 0x6F, 0xBF (up wall)
# 0x6E, 0xBE (down wall)
# 0x2D, 0x2E (up ladder)
# 0x38, 0x39 (down ladder)
# 0xC0, 0x70 (hanging on wall, not moving)

# define patches
patchList:
    PATCH_ID "Climb  " # must be 7 chars
    # 802a06b4 7C 77 1B 78  or       r23,r3,r3
    # r3: ObjInstance* (probably player)
    # r4: PlayerAnimState*
    PATCH_BL 0x802A06B4, main
    # increase climb speeds
    PATCH_HWORD 0x803E8000, 0x3D4B # wall climbing (up and down)
    PATCH_HWORD 0x803E7F84, 0x3CC4 # ladder climbing (up)
    PATCH_HWORD 0x802A26A6, 0x1A84 # ladder climbing (down)
        # last one is changing the down speed to use the same variable
        # as the up speed, because normally it uses a variable that's
        # shared among many different animations.
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


main: # called by our hook, from the patch list.
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r5
    stw   r5, SP_LR_SAVE(r1)
    stmw  r3, SP_GPR_SAVE(r1)

    # set A button icon
    li     r5, 3 # jump
    LOADWH r6, aButtonIcon
    STOREH r5, aButtonIcon, r6

    # prevent immediately regrabbing the wall
    lbz    r5, 0x03F0(r4)
    ori    r5, r5, 0x20
    stb    r5, 0x03F0(r4)

    # check button
    LOADWH  r9, controllerStates
    LOADWL2 r7, buttonsJustPressed, r9 # buttons pressed
    andi.   r5, r7, PAD_BUTTON_A
    beq     .noDrop

    li     r5, 1 # idle
    sth    r5, 0x0274(r4) # set player state

.noDrop:
    lwz  r5, SP_LR_SAVE(r1)
    mtlr r5 # restore LR
    lmw  r3, SP_GPR_SAVE(r1)
    addi r1, r1, STACK_SIZE # restore stack ptr
    or   r23,r3,r3 # replaced
    blr
