# camera patch:
# adds some camera control options.
.text
.include "common.s"
.include "globals.s"

# define patches
patchList:
    PATCH_ID  "Camera " # must be 7 chars
    PATCH_B   0x8010328c, cameraHook
    PATCH_B   0x80014c64, hook_padGetCX
    PATCH_B   0x80014c10, hook_padGetCY
    PATCH_B   0x80133af0, hook_minimap
    PATCH_B   0x801084c8, hook_firstPerson
    PATCH_B   0x8029b03c, hook_aimY
    PATCH_B   0x8029b08c, hook_aimX
    PATCH_BL  0x80108758, hook_viewFinderZoom
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,  0x80 # how much to reserve
    .set SP_LR_SAVE,  0x84
    .set SP_GPR_SAVE, 0x10

entry: # called as soon as our patch is loaded.
    blr

# if we're using the C stick to control the camera, then we must remap the
# functions that currently use it to use the D-pad instead.
hook_padGetCX:
    LOADW   r4,  PATCH_STATE_PTR
    lbz     r4,  CAMERA_OPTIONS(r4)
    andi.   r4,  r4, CAMERA_OPTION_PAD3
    beq     .hook_padGetCX_disabled
    lhz     r4,  0(r3) # get buttons
    li      r3,  0
    andi.   r0,  r4,  PAD_BUTTON_LEFT
    beq     .hook_padGetCX_notLeft
    li      r3,  -16
    blr
.hook_padGetCX_notLeft:
    andi.   r0,  r4,  PAD_BUTTON_RIGHT
    beqlr
    li      r3,  16
    blr
.hook_padGetCX_disabled:
    lbz     r3,  0x4(r3) # get CX
    blr


hook_padGetCY:
    LOADW   r4,  PATCH_STATE_PTR
    lbz     r4,  CAMERA_OPTIONS(r4)
    andi.   r4,  r4, CAMERA_OPTION_PAD3
    beq     .hook_padGetCY_disabled
    lhz     r4,  0(r3) # get buttons
    li      r3,  0
    andi.   r0,  r4,  PAD_BUTTON_UP
    beq     .hook_padGetCY_notUp
    li      r3,  16
    blr
.hook_padGetCY_notUp:
    andi.   r0,  r4,  PAD_BUTTON_DOWN
    beqlr
    li      r3,  -16
    blr
.hook_padGetCY_disabled:
    lbz     r3,  0x5(r3) # get CY
    blr


# since we use the D-pad to control the C menu,
# we must also prevent it from controlling the map.
# we can adjust the map mode using the PDA menu.
# XXX this leaves no way to zoom the map.
hook_minimap: # replaces bl getButtonsHeld
    LOADW   r4,  PATCH_STATE_PTR
    lbz     r4,  CAMERA_OPTIONS(r4)
    andi.   r4,  r4, CAMERA_OPTION_PAD3
    beq     .hook_minimap_disabled
    li      r3,  0 # return no buttons
    li      r31, 0 # where the game stores that
    JUMP    0x80133b00, r0 # skip a call to getButtonsJustPressed

.hook_minimap_disabled:
    CALL    getButtonsJustPressed
    JUMP    0x80133af4, r0 # back to original code


hook_firstPerson:
    lfs     f1, -0x4d20(r2) # replaced
    LOADW   r3,  PATCH_STATE_PTR
    lbz     r3,  CAMERA_OPTIONS(r3)

    andi.   r4,  r3,  CAMERA_OPTION_INVERTX
    beq     .hook_firstPerson_noXInvert
    neg     r28, r28
.hook_firstPerson_noXInvert:
    andi.   r4,  r3,  CAMERA_OPTION_INVERTY
    bne     .hook_firstPerson_noYInvert
    neg     r29, r29
.hook_firstPerson_noYInvert:
    JUMP    0x801084cc, r0


hook_aimY:
    fdivs   f0,  f1,  f0 # replaced
    LOADW   r4,  PATCH_STATE_PTR
    lbz     r4,  CAMERA_OPTIONS(r4)
    andi.   r5,  r4,  CAMERA_OPTION_INVERTY
    bne     .hook_aimY_noInvert
    fneg    f0,  f0
.hook_aimY_noInvert:
    JUMP    0x8029b040, r4

hook_aimX:
    fdivs   f0,  f1,  f0 # replaced
    LOADW   r4,  PATCH_STATE_PTR
    lbz     r4,  CAMERA_OPTIONS(r4)
    andi.   r5,  r4,  CAMERA_OPTION_INVERTX
    beq     .hook_aimX_noInvert
    fneg    f0,  f0
.hook_aimX_noInvert:
    JUMP    0x8029b090, r4


hook_viewFinderZoom:
    # replace "bl padGetCY" in viewfinder controls.
    # remap to L/R, because using D-pad is very slow and not analog.
    LOADW   r4,  PATCH_STATE_PTR
    lbz     r4,  CAMERA_OPTIONS(r4)
    andi.   r4,  r4, CAMERA_OPTION_PAD3
    beq     .hook_viewFinderZoom_disabled

    LOADWH  r9,  controllerStates
    LOADBL2 r5,  controllerStates+6, r9 # L
    LOADBL2 r6,  controllerStates+7, r9 # R
    neg     r6,  r6
    add     r3,  r6,  r5 # return L - R
    blr

.hook_viewFinderZoom_disabled:
    JUMP   0x80014bc4, r0


# hook camera update function to inject our own rotation offsets.
cameraHook:
    bctrl # replaced

    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r5
    stw     r5, SP_LR_SAVE(r1)
    stmw    r3, SP_GPR_SAVE(r1)

    LOADW   r9,  pCamera
    cmpwi   r9,  0
    beq     .noCamera

    LOADWH  r15, controllerStates
    LOADBL2 r3,  (controller1state+4), r15 # CX
    LOADBL2 r4,  (controller1state+5), r15 # CY

    LOADW   r10, PATCH_STATE_PTR
    lbz     r10, CAMERA_OPTIONS(r10)
    andi.   r0,  r10, CAMERA_OPTION_PAD3
    bne     .usePad1

    # XXX use controller 2. the game isn't reading it?
    LOADBL2 r3,  (controller3state+4), r15 # CX
    LOADBL2 r4,  (controller3state+5), r15 # CY
.usePad1:
    # we still use pad 3 for roll because it's rarely needed
    # and there really aren't any buttons left on pad 1 we can use...
    LOADBL2 r5,  (controller3state+6), r15 # L
    LOADBL2 r6,  (controller3state+7), r15 # R
    neg     r5,  r5
    add     r5,  r5,  r6 # r5 = R - L

    # we need to apply a deadzone to the X axis to compensate
    # for the smoothing. otherwise it shudders once it catches
    # up with the analog input.
    andi.   r3,  r3,  0xFC
    extsb   r3,  r3
    extsb   r4,  r4

    # since the game normally doesn't rotate the camera on the X axis,
    # it doesn't do any smoothing there. to prevent jerky movement,
    # we'll implement our own.
    bl .cameraHook_getpc
    .cameraHook_getpc: mflr r14

    lbz     r6,  (prevX - .cameraHook_getpc)(r14)
    extsb   r6,  r6
    # if X == 0, move prevX toward 0
    cmpwi   r3,  0
    beq     .xToZero

    cmpw    r3,  r6
    bgt     .xInc # if X > prevX, increase prevX
    blt     .xDec # if X < prevX, decrease prevX
    b       .doMove

.xToZero:
    cmpwi   r6,  0
    beq     .doMove
    blt     .xToZero_negative
.xDec:
    subi    r6,  r6,  4 # prevX > 0, so, prevX--
    b       .doMove
.xToZero_negative:
.xInc:
    addi    r6,  r6,  4 # prevX < 0, so, prevX++

.doMove:
    stb     r6,  (prevX - .cameraHook_getpc)(r14)
    mr      r3,  r6

    andi.   r0,  r10, CAMERA_OPTION_INVERTX
    beq     .noXInvert
    neg     r3,  r3
.noXInvert:
    andi.   r0,  r10, CAMERA_OPTION_INVERTY
    beq     .noYInvert
    neg     r4,  r4
.noYInvert:

    # scale the movements
    slwi    r3,  r3,  7
    slwi    r4,  r4,  4
    slwi    r5,  r5,  4

    # when holding L, Y rotation is restricted, so we must
    # amplify the rotation we're applying.
    # XXX check for "player is in targeting mode" rather than
    # literally "L button is held".
    LOADBL2 r6,  (controller1state+6), r15 # L
    cmpwi   r6,  16
    blt     .notHoldL
    slwi    r4,  r4,  1

.notHoldL:
    lha     r6,  0x00(r9) # X rotation
    add     r6,  r6,  r3
    sth     r6,  0x00(r9)

    lha     r6,  0x02(r9) # Y rotation
    sub     r6,  r6,  r4  # inverted by default, so double-invert
    sth     r6,  0x02(r9)

    lha     r6,  0x04(r9) # Z rotation
    add     r6,  r6,  r5
    sth     r6,  0x04(r9)

    #lbz     r6,  0x13F(r9)
    #ori     r6,  r6,  7
    #andi.   r6, r6,  0xF8
    #li      r6,  0xFF
    #stb     r6,  0x13F(r9)

.noCamera:
    lwz     r5,  SP_LR_SAVE(r1)
    mtlr    r5   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1, STACK_SIZE # restore stack ptr
    JUMP    0x80103290, r0

prevX: .byte 0 # previous sitck X
