# freemove patch:
# allows the player to be moved arbitrarily using the analog stick
# and L/R buttons.
# toggled by the PDA Menu patch.
.text
.include "common.s"
.include "globals.s"

.macro STOREVEC3F rs, rd, offs
    stfs  \rs,     (\offs)(\rd)
    stfs  (\rs+1), (\offs+4)(\rd)
    stfs  (\rs+2), (\offs+8)(\rd)
.endm

# define patches
patchList:
    PATCH_ID        "FreeMov" # must be 7 chars
    PATCH_MAIN_LOOP mainLoop
    PATCH_BL        0x80105df8, hookFirstPerson
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,0x30 # how much to reserve
    .set SP_LR_SAVE,0x10
    .set SP_OUT_X,0x14
    .set SP_OUT_Y,0x18
    .set SP_OUT_Z,0x1C
    .set SP_FLOAT_TMP,0x20 # temporary storage for float conversion (8 bytes)


entry: # called as soon as our patch is loaded.
    blr


hookFirstPerson:
    LOADW   r5,  PATCH_STATE_PTR
    lbz     r6,  ENABLE_FREE_MOVE(r5)
    cmpwi   r6,  0
    bnelr   # don't enter first person when in free move mode.
    JUMP    0x80103950, r0 # cameraCheckEnterFirstPerson


mainLoop: # called from main loop. r3 = mainLoop
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r4
    stw     r4,  SP_LR_SAVE(r1)
    mr      r14, r3

    # is the PDA menu not open?
    LOADW   r3,  PATCH_STATE_PTR
    lbz     r4,  PDA_MENU_OPEN(r3)
    cmpwi   r4,  0
    bne     .end

    # get object of camera focus.
    LOADW   r21, pCamera
    cmpwi   r21, 0
    beq     .end # no camera
    lwz     r16, 0xA4(r21)
    cmpwi   r16, 0
    beq     .end # no focus object

    # is free move enabled?
    lbz     r4,  ENABLE_FREE_MOVE(r3)
    cmpwi   r4,  0
    beq     .off

        # inhibit C menu
    li      r4,  1
    LOADWH  r5,  shouldCloseCMenu
    STOREB  r4,  shouldCloseCMenu, r5

    # was it just turned on?
    lbz     r5,  (.prevEnable - mainLoop)(r14)
    cmpwi   r5,  0
    bne     .notJustTurnedOn

    li      r3,  1
    stb     r3,  (.prevEnable - mainLoop)(r14)

    # copy player coords to buffer
    lwz     r3,  0x0C(r16)
    stw     r3,  (.freeMoveCoords   - mainLoop)(r14)
    lwz     r3,  0x10(r16)
    stw     r3,  (.freeMoveCoords+4 - mainLoop)(r14)
    lwz     r3,  0x14(r16)
    stw     r3,  (.freeMoveCoords+8 - mainLoop)(r14)

    lhz     r3,  0xB0(r16)
    sth     r3,  (.prevFlagsTarget - mainLoop)(r14)
    ori     r3,  r3, 0xA000 # disable hit detection and update
    sth     r3,  0xB0(r16)

    # also set flags for player in case different from target
    LOADW   r3,  pPlayer
    cmpwi   r3,  0
    beq     .notJustTurnedOn # no player
    lhz     r5,  0xB0(r3)
    sth     r5,  (.prevFlagsPlayer - mainLoop)(r14)
    ori     r5,  r5, 0xA000 # disable hit detection and update
    sth     r5,  0xB0(r3)

.notJustTurnedOn:
    # get the player's angle as X, Z multipliers
    lha     r3, 0(r16)
    addi    r4, r1, SP_OUT_X
    addi    r5, r1, SP_OUT_Z
    CALL    angleToVec2
    lfs     f4, SP_OUT_X(r1)
    lfs     f5, SP_OUT_Z(r1)

    lha     r3, 0(r16)
    addi    r3, r3, 0x4000
    addi    r4, r1, SP_OUT_X
    addi    r5, r1, SP_OUT_Z
    CALL    angleToVec2
    lfs     f6, SP_OUT_X(r1)
    lfs     f7, SP_OUT_Z(r1)

    # f4 = player forward  X movement scale
    # f5 = player forward  Z movement scale
    # f6 = player sideways X movement scale
    # f7 = player sideways Z movement scale

    # do the move
    LOADWH  r9, controllerStates
    LOADBL2 r3, controllerStates+2, r9 # stick X
    LOADBL2 r4, controllerStates+3, r9 # stick Y
    extsb   r3, r3 # sign extend (PPC Y U NO LBA OPCODE!?)
    extsb   r4, r4
    LOADBL2 r5, controllerStates+6, r9 # L
    LOADBL2 r6, controllerStates+7, r9 # R
    neg     r5, r5
    add     r5, r5, r6 # r5 = R - L

    # press B to end free move
    LOADHL2 r6, controllerStates, r9 # buttons
    andi.   r6, r6, PAD_BUTTON_B
    beq     .notBheld
    LOADW   r3, PATCH_STATE_PTR
    li      r4, 0
    stb     r4, ENABLE_FREE_MOVE(r3)
    b       .end
.notBheld:
    # convert stick position to floats
    # adapted from http://mirror.informatimago.com/next/developer.apple.com/documentation/mac/PPCNumerics/PPCNumerics-157.html
    lfd     f9, (.floatMagic - mainLoop)(r14) # load constant into f9
    lfs     f8, (.freeMoveScale - mainLoop)(r14)
    LOADHL2 r6, controllerStates, r9 # buttons
    andi.   r6, r6, PAD_BUTTON_Z
    beq     .notZheld
    lfs     f8, (.freeMoveScale2 - mainLoop)(r14)
.notZheld:
    lis     r6, 0x4330
    stw     r6, SP_FLOAT_TMP(r1) # store exponent part for integer

    # convert stick X to move X
    xoris r3, r3, 0x8000         # invert sign of integer
    stw   r3, SP_FLOAT_TMP+4(r1) # store fraction part for integer
    lfd   f1, SP_FLOAT_TMP(r1)   # load integer in double format into f1
    fsub  f1, f1, f9             # f1 contains converted integer
    frsp  f1, f1 # double to single
    fmuls f1, f1, f8

    # convert stick Y to move Z
    xoris r4, r4, 0x8000
    stw   r4, SP_FLOAT_TMP+4(r1)
    lfd   f3, SP_FLOAT_TMP(r1)
    fsub  f3, f9, f3
    frsp  f3, f3
    fmuls f3, f3, f8

    # convert L/R to move Y
    xoris r5, r5, 0x8000
    stw   r5, SP_FLOAT_TMP+4(r1)
    lfd   f2, SP_FLOAT_TMP(r1)
    fsub  f2, f2, f9
    frsp  f2, f2
    fmuls f2, f2, f8

    # now: f1=X, f2=Y, f3=Z
    # adjust for player angle
    # final X movement = (f1 * f6) + (f3 * f4)
    # final Z movement = (f3 * f5) + (f1 * f7)
    fmuls f6, f1, f6
    fmuls f7, f1, f7
    fmuls f4, f3, f4
    fmuls f5, f3, f5
    # final X = f6 + f4
    # final Z = f5 + f7
    fadds f1, f6, f4
    fadds f3, f5, f7

    lfs     f4, (.freeMoveCoords   - mainLoop)(r14)
    lfs     f5, (.freeMoveCoords+4 - mainLoop)(r14)
    lfs     f6, (.freeMoveCoords+8 - mainLoop)(r14)

    fadds  f4, f4, f1
    fadds  f5, f5, f2
    fadds  f6, f6, f3

    # save
    STOREVEC3F f4, r14, (.freeMoveCoords   - mainLoop)

    # copy saved coords to player
    # why yes, this *is* nuts.
    # a bunch of these probably can be removed.
    # XXX this probably breaks lots of things if target isn't player.
    lwz   r4, 0xB8(r16)
    lwz   r3, (.freeMoveCoords - mainLoop)(r14)
    STOREVEC3F f4, r16, 0x0C
    STOREVEC3F f4, r16, 0x18
    STOREVEC3F f4, r16, 0x80
    STOREVEC3F f4, r16, 0x8C
    STOREVEC3F f4, r4,  0x0C
    STOREVEC3F f4, r4,  0x18
    STOREVEC3F f4, r4,  0x24
    STOREVEC3F f4, r4,  0x3C
    STOREVEC3F f4, r4,  0x48
    STOREVEC3F f4, r4,  0x54
    #STOREVEC3F f4, r4,  0x6C
    #STOREVEC3F f4, r4,  0x78
    #STOREVEC3F f4, r4,  0x84
    #STOREVEC3F f4, r4,  0x9C
    #STOREVEC3F f4, r4,  0xA8
    STOREVEC3F f4, r4,  0xB4
    STOREVEC3F f4, r4,  0xE8
    STOREVEC3F f4, r4,  0x118
    #STOREVEC3F f4, r4,  0x148
    #STOREVEC3F f4, r4,  0x178
    stfs  f5, 0x1BC(r4) # Y only
    stfs  f5, 0x1F4(r4) # Y only

    # if we don't change r16+0C, the player won't move, but the
    # camera will. could make a free camera mode...

    # use C stick to rotate
    # the animState rotation overrides the object rotation
    # but we update both, for non-player objects
    lwz     r4, 0xB8(r16) # animState
    LOADBL2 r3, controllerStates+4, r9 # CX
    extsb   r3, r3
    mulli   r3, r3, -16
    lha     r5, 0x0478(r4)
    add     r5, r5, r3
    sth     r5, 0x0478(r4)
    sth     r5, 0x0000(r16)
    LOADBL2 r3, controllerStates+5, r9 # CY
    extsb   r3, r3
    mulli   r3, r3, -16
    lha     r5, 0x02(r16)
    add     r5, r5, r3
    sth     r5, 0x02(r16)

    # force player state even if target object isn't player,
    # so that the player isn't running around while we're
    # moving some other object.
    LOADW   r3,  pPlayer
    cmpwi   r3,  0
    beq     .end # no player
    lwz     r4,  0xB8(r3) # animState
    li      r5,  1
    sth     r5,  0x0274(r4)

    # force camera behind player.
    lha     r3,  0(r16)
    addi    r4,  r1, SP_OUT_X
    addi    r5,  r1, SP_OUT_Z
    CALL    angleToVec2
    lfs     f1,  (.camDist - mainLoop)(r14)
    addi    r3,  r1, SP_OUT_X
    mr      r4,  r4
    CALL    vec3f_scale
    lfs     f4,  SP_OUT_X(r1)
    lfs     f5,  SP_OUT_Z(r1)

    LOADW   r4,  pCamera
    stfs    f4,  0x0C(r4)
    stfs    f5,  0x14(r4)
    lfs     f1,  0x10(r16)
    stfs    f1,  0x10(r4)

.end:
    lwz  r3,  SP_LR_SAVE(r1)
    mtlr r3   # restore LR
    addi r1,  r1,  STACK_SIZE # restore stack ptr
    blr

.off:
    lbz   r5,  (.prevEnable - mainLoop)(r14)
    cmpwi r5,  0
    beq   .notJustTurnedOff

    # restore player flags
    LOADW   r3,  pPlayer
    cmpwi   r3,  0
    beq     .notJustTurnedOff # no player
    lhz     r4,  (.prevFlagsPlayer - mainLoop)(r14)
    sth     r4,  0xB0(r3)

    # restore target flags
    lhz     r4,  (.prevFlagsTarget - mainLoop)(r14)
    sth     r4,  0xB0(r16)

.notJustTurnedOff:
    li      r3,  0
    stb     r3,  (.prevEnable - mainLoop)(r14)
    b       .end

.camDist:         .float 15 # distance for camera to follow behind player
.floatMagic:      .int 0x43300000,0x80000000
.freeMoveScale:   .float 0.07
.freeMoveScale2:  .float 0.35 # when Z held
.freeMoveCoords:  .int 0, 0, 0
.prevFlagsTarget: .short 0
.prevFlagsPlayer: .short 0
.prevEnable:      .byte 0
