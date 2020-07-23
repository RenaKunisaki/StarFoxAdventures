# freemove patch:
# allows the player to be moved arbitrarily using the analog stick
# and L/R buttons.
# toggled by the PDA Menu patch.
.text
.include "common.s"
.include "globals.s"

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

    # get object of camera focus.
    LOADW   r21, pCamera
    cmpwi   r21, 0
    beq     .end # no camera
    lwz     r16, 0xA4(r21)
    cmpwi   r16, 0
    beq     .end # no focus object

    # is free move enabled?
    LOADW   r3,  PATCH_STATE_PTR
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

    # XXX force camera behind player
    # this method doesn't work
    #li      r3,  0x3F80
    #stw     r3,  0xF4(r21)
    # would just copying target's rotation to camera work?
    # if it rotates around target rather than its own position...

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
    stfs    f4, (.freeMoveCoords   - mainLoop)(r14)
    stfs    f5, (.freeMoveCoords+4 - mainLoop)(r14)
    stfs    f6, (.freeMoveCoords+8 - mainLoop)(r14)

    # copy saved coords to player
    lwz   r3, (.freeMoveCoords - mainLoop)(r14)
    stw   r3, 0x0C(r16)
    #stw   r3, 0x18(r16)
    lwz   r3, (.freeMoveCoords+4 - mainLoop)(r14)
    stw   r3, 0x10(r16)
    #stw   r3, 0x1C(r16)
    lwz   r3, (.freeMoveCoords+8 - mainLoop)(r14)
    stw   r3, 0x14(r16)
    #stw   r3, 0x20(r16)

    # use C stick to rotate
    # the animState rotation overrides the object rotation
    # but we update both, for non-player objects
    lwz     r4, 0xB8(r16) # animState
    LOADBL2 r3, controllerStates+4, r9 # CX
    extsb   r3, r3
    mulli   r3, r3, -8
    lha     r5, 0x0478(r4)
    add     r5, r5, r3
    sth     r5, 0x0478(r4)
    sth     r5, 0x0000(r16)
    LOADBL2 r3, controllerStates+5, r9 # CY
    extsb   r3, r3
    mulli   r3, r3, -8
    lha     r5, 0x02(r16)
    add     r5, r5, r3
    sth     r5, 0x02(r16)

    # force player state
    li    r5, 1
    sth   r5, 0x0274(r4)
    #li    r5, 0
    #stw   r5, 0x98(r16) # clear animTimer
    #stw   r5, 0x9C(r16) # clear anim something
    #sth   r5, 0xA0(r16) # clear animID
    #lhz   r5, 0xB0(r16)
    #ori   r5, r5, 0x8000 # disable update
    #sth   r5, 0xB0(r16)

    # force player state even if target object isn't player,
    # so that the player isn't running around while we're
    # moving some other object.
    LOADW   r3,  pPlayer
    cmpwi   r3,  0
    beq     .end # no player
    lwz     r4, 0xB8(r3) # animState
    li      r5, 1
    sth     r5, 0x0274(r4)
    #li      r5, 0
    #stw     r5, 0x98(r3) # clear animTimer
    #stw     r5, 0x9C(r3) # clear anim something
    #sth     r5, 0xA0(r3) # clear animID
    #lhz     r5, 0xB0(r3)
    #ori     r5, r5, 0x8000 # disable update
    #sth     r5, 0xB0(r3)

    li      r4,  0
    LOADW   r5,  pCamera
    stw     r4,  0xF4(r5)

    # this only applies to collisions with objects, not geometry.
    #stw     r5,  0x54(r3) # hitbox
    #cmpwi   r5,  0
    #beq     .noHitbox
    #lhz     r6,  0x60(r5)
    #ori     r6,  r6,  0x01
    #sth     r6,  0x60(r5)
.noHitbox:

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

.floatMagic:      .int 0x43300000,0x80000000
.freeMoveScale:   .float 0.07
.freeMoveScale2:  .float 0.35 # when Z held
.freeMoveCoords:  .int 0, 0, 0
.prevFlagsTarget: .short 0
.prevFlagsPlayer: .short 0
.prevEnable:      .byte 0
