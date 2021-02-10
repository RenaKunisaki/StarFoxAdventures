# rumble patch:
# activates blur filter when controller rumbles.
.text
.include "common.s"
.include "globals.s"

# define patches
patchList:
    PATCH_ID "Rumble " # must be 7 chars
    PATCH_BL 0x8005c45c, main
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,0x100 # how much to reserve
    .set SP_LR_SAVE,0x104 # this is what the game does
    .set SP_GPR_SAVE,0x10

entry: # called as soon as our patch is loaded.
    # nothing to do
    blr


main: # called by our hook, from the patch list.
    # replaced a call to 8000e620: BOOL shouldForceMotionBlur()
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r5
    stw     r5,  SP_LR_SAVE(r1)
    stmw    r4,  SP_GPR_SAVE(r1)

    # is this enabled?
    LOADW   r3,  PATCH_STATE_PTR
    lbz     r4,  EXTRA_FEATURE_FLAGS(r3)
    andi.   r4,  r4,  EXTRA_FEATURE_RUMBLE_BLUR
    beq     .disabled

    bl      .getpc
    .getpc: mflr r5

    # is rumble currently active?
    LOADWH  r4,  rumbleTimer
    LOADWL2 r3,  rumbleTimer, r4
    cmpwi   r3,  0
    bne     .doOverride

    # rumble timer is 0 - was it > 0 on previous frame?
    lbz     r4,  (.wasOn - .getpc)(r5)
    stb     r3,  (.wasOn - .getpc)(r5)
    cmpwi   r4,  0
    beq     .disabled

    # make sure we turn blur back off
    LOADWH  r4,  motionBlurIntensity
    STOREW  r3,  motionBlurIntensity, r4

.disabled:
    CALL    0x8000e620 # replaced function
    b       .end # restore the stack

.doOverride: # apply blur
    lfs     f2,  (.scale  - .getpc)(r5)
    lfs     f3,  (.maxVal - .getpc)(r5)
    lfs     f4,  (.minVal - .getpc)(r5)
    LOADFL2 f1,  rumbleTimer, r4
    fmuls   f1,  f1,  f2
    fsubs   f1,  f3,  f1
    fcmpo   0,   f1,  f4
    bgt     .doBlur
    fmr     f1,  f4
.doBlur:
    LOADWH  r4,  motionBlurIntensity
    STOREF  f1,  motionBlurIntensity, r4
    li      r3,  1 # do force
    stb     r3,  (.wasOn - .getpc)(r5)

.end:
    lwz     r5,  SP_LR_SAVE(r1)
    mtlr    r5   # restore LR
    lmw     r4,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    blr

.scale:  .float 2.0
.minVal: .float 48.0
.maxVal: .float 120.0
.wasOn:  .byte 0
