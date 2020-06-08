# debugprint patch:
# 1) patches into main loop to display some info on screen
# 2) moves debug print to very edges of screen
.text
.include "common.s"
.include "globals.s"

# define patches
patchList:
    PATCH_ID        "DbgText" # must be 7 chars
    PATCH_MAIN_LOOP mainLoop
    # patch debug print to move to the very edge of the screen.
    PATCH_HWORD 0x8013761A, 0
    PATCH_HWORD 0x8013762E, 0
    PATCH_HWORD 0x8013764A, 0
    PATCH_HWORD 0x8013765E, 0
    # make fixed width more reasonable
    PATCH_BYTE  0x80137317, 6
    # enable debug functions on controller 3
    PATCH_BYTE 0x80014E73, 0x00000004
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,0x40 # how much to reserve
    .set SP_LR_SAVE,0x10
    .set SP_R14_SAVE,0x14
    .set SP_R15_SAVE,0x18
    .set SP_R16_SAVE,0x1C
    .set SP_FLOAT_TMP,0x20 # temporary storage for float conversion (8 bytes)

entry: # called as soon as our patch is loaded.
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r3
    stw   r3,  SP_LR_SAVE(r1)

    # print some info about boot environment.
    lis    r3,  0x8000
    lwz    r4,  0x28(r3) # system memory size
    lwz    r5,  0xF0(r3) # simulated memory size
    lwz    r6,  0xD0(r3) # ARAM size
    lwz    r7,  0xE8(r3) # debug monitor size
    lwz    r8,  0xEC(r3) # debug monitor addr
    lwz    r9,  0x30(r3) # arena lo
    lwz    r10, 0x34(r3) # arena hi

    bl .entry_getpc
    .entry_getpc:
        mflr r3
    addi r3, r3, (bootMsg - .entry_getpc)@l
    CALL OSReport

    lwz  r3,  SP_LR_SAVE(r1)
    mtlr r3 # restore LR
    addi r1, r1, STACK_SIZE # restore stack ptr
    blr

mainLoop: # called by our hook, from the patch list.
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r3
    stw   r3,  SP_LR_SAVE(r1)
    stw   r14, SP_R14_SAVE(r1)
    stw   r15, SP_R15_SAVE(r1)
    stw   r16, SP_R16_SAVE(r1)

    # the patch that restores debug print functions also overrides
    # this variable, so manually check it before doing things.
    # XXX that patch should be integrated into this one instead
    # of being a separate Gecko code.
    LOADB r4, enableDebugText
    cmpwi r4, 0
    beq   .end

    bl .getpc
    .getpc:
        mflr r14

    # display heap stats: free bytes, free blocks
    LOAD  r16, 0x803406A0 # heap 0
    li    r15, 0 # idx
.nextHeap:
    addi  r3, r14, (.fmt_heap - .getpc)@l
    #mr    r4, r15 # heap num

    lwz   r4, 0x0C(r16) # total bytes
    lwz   r5, 0x00(r16) # total blocks
    lwz   r6, 0x10(r16) # used bytes
    #sub   r4, r4, r6    # free bytes
    mulli r6, r6, 100
    divwu r4, r6, r4 # used bytes percent
    lwz   r6, 0x04(r16) # used blocks
    #sub   r5, r5, r6    # free blocks
    mulli r6, r6, 100
    divwu r5, r6, r5 # used blocks percent

    CALL debugPrintf
    addi  r16, r16, 0x14 # heap entry size
    addi  r15, r15, 1
    cmpwi r15, 4
    bne   .nextHeap


    # get player object
    LOADW r16, pPlayer
    cmpwi r16, 0
    beq   .noPlayer

    # display nearby object
    # this doesn't work because this function doesn't
    # actually give us the nearest object, it does something
    # with specific objects.
    # if we were to iterate all objects, we'd need to filter for things
    # like the staff, shield, backpack...
    #li    r3, 0 # idx
    #ori   r4, r16, 0 # obj
    #li    r5, 0 # outDistance
    #CALL  playerGetNearestObject
    #ori   r10, r3, 0 # param for %p
    #li    r17, 0
    #cmpwi r3, 0
    #beq   .noObject
    #lwz   r3, 0x50(r3) # get file ptr
    #addi  r17, r3, 0x91 # get name

#.noObject:

    addi r3, r14, (.fmt_playerCoords - .getpc)@l

    # display player coords
    # debugPrintf doesn't support eg '%+7.2f' so we'll just convert
    # to int to get rid of excess digits
    lfs    f1, 0x0C(r16)
    fctiwz f2,f1
    stfd   f2,SP_FLOAT_TMP(r1)
    lwz    r4,SP_FLOAT_TMP+4(r1)

    lfs    f1, 0x10(r16)
    fctiwz f2,f1
    stfd   f2,SP_FLOAT_TMP(r1)
    lwz    r5,SP_FLOAT_TMP+4(r1)

    lfs    f1, 0x14(r16)
    fctiwz f2,f1
    stfd   f2,SP_FLOAT_TMP(r1)
    lwz    r6,SP_FLOAT_TMP+4(r1)

    #lfs   f2, 0x10(r16)
    #lfs   f3, 0x14(r16)
    ## magic required to make floats print correctly
    ## no idea what this does
    #creqv 4*cr1+eq,4*cr1+eq,4*cr1+eq

    CALL debugPrintf

    # display map cell
    addi r3, r14, (.fmt_mapCoords - .getpc)@l
    LOADWH  r9, mapCoords
    LOADWL2 r4, mapCoords,    r9
    LOADWL2 r5, mapCoords+ 4, r9
    LOADWL2 r6, mapCoords+ 8, r9
    LOADWL2 r7, mapCoords+12, r9
    #ori     r8, r10, 0 # r9 = nearest object
    #ori     r9, r17, 0 # r10 = name
    CALL debugPrintf

    # display player state
    addi r3, r14, (.fmt_playerState - .getpc)@l
    lwz  r4, 0x00B8(r16) # get animState
    lfs  f1, 0x0098(r16) # get anim timer
    lfs  f2, 0x0814(r4)  # get anim val
    lhz  r4, 0x0274(r4)  # get state ID
    lhz  r5, 0x00A0(r16) # get anim ID
    # magic required to make floats print correctly
    # no idea what this does
    creqv 4*cr1+eq,4*cr1+eq,4*cr1+eq
    CALL debugPrintf

    # display input
    LOADWH  r6, controllerStates
    LOADBL2 r4, controllerStates+2, r6 # x
    LOADBL2 r5, controllerStates+3, r6 # y
    addi r3, r14, (.fmt_input - .getpc)@l
    CALL debugPrintf

    # do free movement (XXX move to another file)

    # check toggle button
    lbz     r15, (.freeMoveEnabled - .getpc)(r14)
    LOADWH  r9, controllerStates
    LOADWL2 r7, buttonsJustPressed, r9 # buttons pressed
    LOADHL2 r8, controllerStates, r9 # buttons held
    andi.   r3, r8, PAD_BUTTON_L | PAD_BUTTON_R
    beq     .notToggle
    # L+R held, B pressed?
    andi.   r3, r7, PAD_BUTTON_B
    beq     .notToggle

    # toggle free move
    xori   r15, r15, 1
    stb    r15, (.freeMoveEnabled - .getpc)(r14)
    # don't lock player. it locks camera too, and causes the
    # shielding sound effect to not stop.
    #LOADWH r3,  playerLocked
    #STOREB r15, playerLocked, r3
    li     r3, 0
    li     r4, 0x3EB
    CALL   audioPlaySound

    # copy player coords to buffer
    lwz   r3, 0x0C(r16)
    stw   r3, (.freeMoveCoords   - .getpc)(r14)
    lwz   r3, 0x10(r16)
    stw   r3, (.freeMoveCoords+4 - .getpc)(r14)
    lwz   r3, 0x14(r16)
    stw   r3, (.freeMoveCoords+8 - .getpc)(r14)

.notToggle:
    cmpwi r15, 0 # free move enabled?
    beq   .noFreeMove

    # do the move
    lfs     f1, (.freeMoveCoords   - .getpc)(r14)
    lfs     f2, (.freeMoveCoords+4 - .getpc)(r14)
    lfs     f3, (.freeMoveCoords+8 - .getpc)(r14)
    LOADBL2 r3, controllerStates+2, r9 # stick X
    LOADBL2 r4, controllerStates+3, r9 # stick Y
    extsb   r3, r3 # sign extend (PPC Y U NO LBA OPCODE!?)
    extsb   r4, r4
    LOADBL2 r5, controllerStates+6, r9 # L
    LOADBL2 r6, controllerStates+7, r9 # R
    neg     r5, r5
    add     r5, r5, r6 # r5 = R - L

    # convert stick position to floats
    # adapted from http://mirror.informatimago.com/next/developer.apple.com/documentation/mac/PPCNumerics/PPCNumerics-157.html
    lfd   f9, (.floatMagic - .getpc)(r14) # load constant into f9
    lfs   f8, (.freeMoveScale - .getpc)(r14)
    lis   r6, 0x4330
    stw   r6, SP_FLOAT_TMP(r1) # store exponent part for integer

    # convert stick X
    xoris r3, r3, 0x8000         # invert sign of integer
    stw   r3, SP_FLOAT_TMP+4(r1) # store fraction part for integer
    lfd   f4, SP_FLOAT_TMP(r1)   # load integer in double format into f4
    fsub  f4, f4, f9             # f4 contains converted integer
    frsp  f4, f4 # double to single
    fmuls f4, f4, f8

    # convert stick Y
    xoris r4, r4, 0x8000
    stw   r4, SP_FLOAT_TMP+4(r1)
    lfd   f5, SP_FLOAT_TMP(r1)
    fsub  f5, f5, f9
    frsp  f5, f5
    fmuls f5, f5, f8

    # convert L/R
    xoris r5, r5, 0x8000
    stw   r5, SP_FLOAT_TMP+4(r1)
    lfd   f6, SP_FLOAT_TMP(r1)
    fsub  f6, f6, f9
    frsp  f6, f6
    fmuls f6, f6, f8

    # add to player
    # XXX account for camera rotation
    fadds  f1, f1, f4
    fadds  f2, f2, f6
    fadds  f3, f3, f5

    # save
    stfs    f1, (.freeMoveCoords   - .getpc)(r14)
    stfs    f2, (.freeMoveCoords+4 - .getpc)(r14)
    stfs    f3, (.freeMoveCoords+8 - .getpc)(r14)

    # copy saved coords to player
    lwz   r3, (.freeMoveCoords - .getpc)(r14)
    stw   r3, 0x0C(r16)
    #stw   r3, 0x18(r16)
    lwz   r3, (.freeMoveCoords+4 - .getpc)(r14)
    stw   r3, 0x10(r16)
    #stw   r3, 0x1C(r16)
    lwz   r3, (.freeMoveCoords+8 - .getpc)(r14)
    stw   r3, 0x14(r16)
    #stw   r3, 0x20(r16)

.noFreeMove:


.noPlayer:
.end:
    lwz  r14, SP_R14_SAVE(r1)
    lwz  r15, SP_R15_SAVE(r1)
    lwz  r16, SP_R16_SAVE(r1)
    lwz  r3,  SP_LR_SAVE(r1)
    mtlr r3 # restore LR
    addi r1, r1, STACK_SIZE # restore stack ptr
    blr

.floatMagic:
    .int 0x43300000,0x80000000
.freeMoveScale:
    .float 0.07

.freeMoveCoords:
    .int 0, 0, 0

.freeMoveEnabled:
    .byte 0

.fmt_heap:
    .string "\x84%3d %3d\x83, "
.fmt_playerCoords:
    .string "\nP \x84%d %d %d\x83 "
.fmt_mapCoords:
    .string "M \x84%d %d %d %d\x83 "
.fmt_playerState:
    .string "\nS \x84%02X\x83 A \x84%04X %f %f\x83\n"
    #.string "S \x84%02X\x83 A \x84%04X\x83\n"
.fmt_input:
    .string "%X %X\n"
bootMsg:
    .string "Mem size %08X (sim %08X), ARAM %08X, monitor %08X @ %08X, arena %08X - %08X"
