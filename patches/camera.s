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
    PATCH_B   0x801030c0, cameraUpdate
    PATCH_B   0x8002e97c, camUpdateOverrideTimeStop
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,   0xC0 # how much to reserve
    .set SP_LR_SAVE,   0xC4
    .set SP_FLOAT_TMP, 0x10
    .set SP_CAM_DIST,  0x20
    .set SP_GPR_SAVE,  0x30

entry: # called as soon as our patch is loaded.
    blr

intToFloat: # convert r3 from int to float.
    # stores result in f1.
    # requires magic constant in f9: .int 0x43300000,0x80000000
    # clobbers r0, r3, SP_FLOAT_TMP
    #lfd     f9,  (floatMagic - mainLoop)(r14) # load constant into f9
    lis     r0,  0x4330
    stw     r0,  SP_FLOAT_TMP(r1)   # store exponent part for integer
    xoris   r3,  r3,  0x8000        # invert sign of integer
    stw     r3,  SP_FLOAT_TMP+4(r1) # store fraction part for integer
    lfd     f1,  SP_FLOAT_TMP(r1)   # load integer in double format into f1
    fsub    f1,  f1,  f9            # f1 contains converted integer
    frsp    f1,  f1                 # double to single
    blr


doMoveXZ:
    # expects r15 = camera, r3 = forward motion, r4 = sideways motion
    # this is a subroutine because I originally planned to go a different
    # way with it and can't be arsed to change it.
    mflr    r29

    # convert motion vector to floats-
    bl      intToFloat
    stfs    f1,  (SP_CAM_DIST + 0x00)(r1) # +00 = fwd
    mr      r3,  r4
    bl      intToFloat
    stfs    f1,  (SP_CAM_DIST + 0x04)(r1) # +04 = sideways

    # convert rotation to radians
    lha     r3,  0x00(r15) # get rotation (0-65535)
    addi    r3,  r3,  0x4000 # not sure why we need this...
    bl      intToFloat # f1 = rotation (0-65535.0)
    LOADWH  r9,  0x803de7d8 # magic constant s16 to radians
    LOADFDL2 f2,  0x803de7d8, r9
    fdivs   f1,  f1,  f2 # f1 = rotation in radians
    stfs    f1,  (SP_FLOAT_TMP + 0x00)(r1)

    # compute sin(r) and cos(r)
    CALL    sinf
    stfs    f1,  (SP_CAM_DIST + 0x08)(r1) # +08 = sin(r)
    lfs     f1,  (SP_FLOAT_TMP + 0x00)(r1)
    CALL    cosf

    fmr     f4,  f1                       # f4 = cos(r)
    lfs     f5,  (SP_CAM_DIST + 0x08)(r1) # f5 = sin(r)
    lfs     f6,  (SP_CAM_DIST + 0x00)(r1) # f6 = fwd
    lfs     f7,  (SP_CAM_DIST + 0x04)(r1) # f7 = side

    # x += (fwd * cos(r)) - (side * sin(r))
    fmuls   f1,  f6,  f4 # f1 = fwd  * cos(r)
    fmuls   f2,  f7,  f5 # f2 = side * sin(r)
    fsubs   f1,  f1,  f2
    fmr     f8,  f1
    lfs     f2,  0x18(r15)
    fadds   f2,  f2,  f1
    stfs    f2,  0x18(r15)

    # z += (fwd * sin(r)) + (side * cos(r))
    fmuls   f1,  f6,  f5 # f1 = fwd  * sin(r)
    fmuls   f2,  f7,  f4 # f2 = side * cos(r)
    fadds   f1,  f1,  f2
    fmr     f9,  f1
    lfs     f2,  0x20(r15)
    fadds   f2,  f2,  f1
    stfs    f2,  0x20(r15)

    mtlr    r29
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



camUpdateOverrideTimeStop:
    # hook the end of updateObjects()
    # if time is stopped, the camera won't update.
    # we need to override that when debug mode is on.
    addi    r11, r1,  0x30 # replaced
    cmpwi   r29, 0 # is time stopped?
    beq     .timeStopOverrideEnd # nope, don't do anything.

    # is the camera in debug mode?
    LOADW   r29, PATCH_STATE_PTR
    lbz     r0,  DEBUG_CAM_MODE(r29)
    cmpwi   r0,  0
    beq     .timeStopOverrideEnd # nope, still don't do anything.

    # debug mode is active, so override the time stop.
    mflr    r29
    CALL    0x801030c0 # update the camera
    mtlr    r29

.timeStopOverrideEnd:
    JUMP    0x8002e980, r0 # back to original code


# hook the entire camera update function for debug stuff.
cameraUpdate: # r3: frames
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r4
    stw     r4,  SP_LR_SAVE(r1)

    bl .cameraUpdate_getpc
    .cameraUpdate_getpc: mflr r14
    mtlr    r4

    LOADW   r15, PATCH_STATE_PTR
    lbz     r0,  DEBUG_CAM_MODE(r15)
    cmpwi   r0,  0
    bne     .cameraUpdate_debug

    # jump back to original update function.
    lwz     r5,  SP_LR_SAVE(r1)
    mtlr    r5   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1, STACK_SIZE # restore stack ptr
    stwu    r1,  -0x10(r1) # replaced
    JUMP    0x801030c4, r0

.cameraUpdate_skip:
    # draw debug info
    LOADW   r15, pCamera
    addi    r5,  r14, s_xpos - .cameraUpdate_getpc
    lfs     f1,  0x18(r15)
    fctiwz  f1,  f1
    stfd    f1,  SP_FLOAT_TMP(r1)
    lwz     r6,  (SP_FLOAT_TMP+4)(r1)
    lhz     r7,  0x00(r15)
    li      r3,  24 # X
    li      r4,  400 # Y
    CALL    debugPrintfxy

    addi    r5,  r14, s_ypos - .cameraUpdate_getpc
    lfs     f1,  0x1C(r15)
    fctiwz  f1,  f1
    stfd    f1,  SP_FLOAT_TMP(r1)
    lwz     r6,  (SP_FLOAT_TMP+4)(r1)
    lhz     r7,  0x02(r15)
    li      r3,  24 # X
    li      r4,  411 # Y
    CALL    debugPrintfxy

    addi    r5,  r14, s_zpos - .cameraUpdate_getpc
    lfs     f1,  0x20(r15)
    fctiwz  f1,  f1
    stfd    f1,  SP_FLOAT_TMP(r1)
    lwz     r6,  (SP_FLOAT_TMP+4)(r1)
    lhz     r7,  0x04(r15)
    li      r3,  24 # X
    li      r4,  422 # Y
    CALL    debugPrintfxy

    # skip original update function.
    lwz     r5,  SP_LR_SAVE(r1)
    mtlr    r5   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1, STACK_SIZE # restore stack ptr
    blr


.cameraUpdate_debug:
    LOADW   r15, pCamera
    lwz     r16, 0xA4(r15) # focused object
    cmpwi   r0,  DEBUG_CAM_STAY
    beq     doDebugCamStay
    # else must be free mode

doDebugCamFree:
    #   SY:    move forward/backward
    #   SX:    turn left/right
    #   CX:    tilt up/down
    #   CY:    move left/right
    #   L/R:   move up/down
    #   start: toggle time stop
    # * X:     toggle HUD/debug text
    # * Y:     reset rotation
    # hold Z to modify:
    # * CX:    roll (or just keep using pad 3 L/R for this?)
    # * start: advance frame
    # * Y:     move camera back to player
    # * X:     move player to camera
    #   else:  same but faster
    # * not implemented yet

    # we need this for intToFloat
    lfd     f9,  (floatMagic - .cameraUpdate_getpc)(r14)

    LOADWH  r17, controllerStates
    LOADHL2 r3,  (controller4state+0), r17 # buttons
    srwi    r3,  r3,  4
    andi.   r9,  r3,  1 # isolate Z button

    # turn left/right
    LOADBL2 r3,  (controller4state+2), r17 # left stick X
    extsb   r3,  r3
    lha     r4,  0x00(r15)
    slwi    r3,  r3,  1
    slw     r3,  r3,  r9 # double rotation if Z held
    add     r4,  r4,  r3
    sth     r4,  0x00(r15)

    # tilt up/down
    LOADBL2 r3,  (controller4state+5), r17 # CY
    extsb   r3,  r3
    lha     r4,  0x02(r15)
    slwi    r3,  r3,  1
    slw     r3,  r3,  r9 # double rotation if Z held
    add     r4,  r4,  r3
    sth     r4,  0x02(r15)

    # move up/down
    LOADBL2 r5,  (controller4state+6), r17 # L
    LOADBL2 r6,  (controller4state+7), r17 # R
    neg     r5,  r5
    add     r5,  r5,  r6 # r5 = R - L
    slw     r3,  r5,  r9 # double speed if Z held
    li      r4,  16
    divw    r3,  r3,  r4 # don't be crazy fast
    bl      intToFloat
    lfs     f2,  0x1C(r15)
    fadds   f2,  f2,  f1
    stfs    f2,  0x1C(r15)

    # move forward/back/left/right
    li      r5,  16
    LOADBL2 r3,  (controller4state+3), r17 # left stick Y
    extsb   r3,  r3
    slw     r3,  r3,  r9 # double speed if Z held
    divw    r3,  r3,  r5 # don't be crazy fast
    LOADBL2 r4,  (controller4state+4), r17 # CX
    extsb   r4,  r4
    slw     r4,  r4,  r9 # double speed if Z held
    divw    r4,  r4,  r5 # don't be crazy fast
    bl      doMoveXZ

    # check buttons
    lbz     r3,  (padDelay - .cameraUpdate_getpc)(r14)
    cmpwi   r3,  0
    beq     .debugCamCheckPad
    subi    r3,  r3,  1
    stb     r3,  (padDelay - .cameraUpdate_getpc)(r14)
    b       .debugCamSkipPad

    .debugCamCheckPad:
    LOADHL2 r3,  (controller4state+0), r17 # buttons
    andi.   r0,  r3,  PAD_BUTTON_MENU
    bne     .debugCamToggleTimeStop

.debugCamSkipPad:
    # now we need to update the actual view matrix to match the camera.
    mr      r3,  r15
    CALL    0x80101980 # cameraUpdateViewMtx
    b       .cameraUpdate_skip

.debugCamToggleTimeStop:
    LOADWH  r3,  timeStop
    LOADBL2 r4,  timeStop, r3
    xori    r4,  r4,  1
    STOREB  r4,  timeStop, r3
    b       .debugCamDoneInput

.debugCamDoneInput:
    li      r3,  120
    stb     r3,  (padDelay - .cameraUpdate_getpc)(r14)
    b       .debugCamSkipPad


doDebugCamStay:
    # same as free, but still rotate to point to target.
    lfs     f1,  (f_zero - .cameraUpdate_getpc)(r14) # Y offset
    mr      r3,  r15  # camera
    addi    r4,  r1,  SP_CAM_DIST + 0x00 # outX
    addi    r5,  r1,  SP_CAM_DIST + 0x04 # outY
    addi    r6,  r1,  SP_CAM_DIST + 0x08 # outZ
    addi    r7,  r1,  SP_CAM_DIST + 0x0C # outXZ
    li      r8,  0 # bUseCurPos
    CALL    cameraGetFocusObjDistance

    # pan to point to player (or whatever is focused)
    # (X rotation value, even though it's Y-axis rotation...)
    lfs     f1,  (SP_CAM_DIST + 0x00)(r1) # outX
    lfs     f2,  (SP_CAM_DIST + 0x08)(r1) # outZ
    CALL    atan2 # now r3 = 0-65535 angle

    # not exactly sure why this is necessary.
    li      r4,  -0x8000
    sub     r3,  r4,  r3
    sth     r3,  0x00(r15)

    # tilt to point to player
    # (Y rotation value, even though it's local X-axis rotation...)
    # XXX not working?
    lfs     f1,  0x10(r15) # cam Y
    lfs     f2,  0x10(r16) # focus Y
    LOADW   r3,  0x803dd530 # target Y offset (look at head, not feet)
    cmpwi   r3,  0
    beq     .debugCamStayNoOffset
    lfs     f3,  0x8C(r3)
    fadds   f2,  f2,  f3
.debugCamStayNoOffset:
    fsubs   f1,  f1,  f2 # camY - focusY
    lfs     f2,  (SP_CAM_DIST + 0x0C)(r1) # distXZ
    CALL    atan2 # now r3 = 0-65535 angle
    sth     r3,  0x02(r15)

    # we don't need to do anything with roll/Z value.

    # still call free mode, so we can move the camera.
    b       doDebugCamFree


floatMagic: .int 0x43300000,0x80000000
f_zero: .float 0
prevX: .byte 0 # previous stick X
padDelay: .byte 9
s_xpos: .string "X\t%6d\t%5d"
s_ypos: .string "Y\t%6d\t%5d"
s_zpos: .string "Z\t%6d\t%5d"
