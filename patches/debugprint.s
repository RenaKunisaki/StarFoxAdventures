# debugprint patch:
# 1) patches into main loop to display some info on screen
# 2) moves debug print to very edges of screen
# 3) draws bars in bottom left corner showing CPU time and memory usage
.text
.include "common.s"
.include "globals.s"

# define patches
patchList:
    PATCH_ID        "DbgText" # must be 7 chars
    PATCH_MAIN_LOOP mainLoop

    # restore some stubbed print functions (redirect to OSReport)
    PATCH_WORD 0x80148B78, 0x4BF34B64 # trickyReportError
    PATCH_WORD 0x802510CC, 0x4BE2C610 # dspDebugPrint
    PATCH_WORD 0x80246E04, 0x4BE368D8 # _osDebugPrint

    # restore debugPrintf
    PATCH_WORD 0x801378A8, 0x480000A0 # debugPrintf
    PATCH_B 0x8013798C, hook_logPrintf

    # patch debug print to move to the very edge of the screen.
    PATCH_HWORD 0x8013761A, 0
    PATCH_HWORD 0x8013762E, 0
    PATCH_HWORD 0x8013764A, 0
    PATCH_HWORD 0x8013765E, 0

    # make fixed width more reasonable
    # we can even do 5 here, but then the letter B gets cut off
    # and looks like 8.
    PATCH_BYTE  0x80137317, 6

    # enable debug functions on controller 3
    PATCH_BYTE 0x80014E73, 0x00000004

    # patch chapter select to just Z button
    PATCH_WORD 0x80119D90, 0x60000000

    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,  0x40 # how much to reserve
    .set SP_LR_SAVE,  0x10
    .set SP_R14_SAVE, 0x14
    .set SP_R15_SAVE, 0x18
    .set SP_R16_SAVE, 0x1C
    .set SP_FLOAT_TMP,0x20 # temporary storage for float conversion (8 bytes)
    .set SP_OBJ_NAME, 0x28 # object name (12 bytes)

entry: # called as soon as our patch is loaded.
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r4
    stw   r4,  SP_LR_SAVE(r1)
    stw   r14, SP_R14_SAVE(r1)
    mr    r14, r3

    # print some info about boot environment.
    lis    r3,  0x8000
    lwz    r4,  0x28(r3) # system memory size
    lwz    r5,  0xF0(r3) # simulated memory size
    lwz    r6,  0xD0(r3) # ARAM size
    lwz    r7,  0xE8(r3) # debug monitor size
    lwz    r8,  0xEC(r3) # debug monitor addr
    lwz    r9,  0x30(r3) # arena lo
    lwz    r10, 0x34(r3) # arena hi

    addi r3, r14, (bootMsg - patchList)@l
    CALL OSReport

    lwz  r14, SP_R14_SAVE(r1)
    lwz  r3,  SP_LR_SAVE(r1)
    mtlr r3 # restore LR
    addi r1, r1, STACK_SIZE # restore stack ptr
    blr


hook_logPrintf: # hooked beginning of logPrintf
    stw     r10, 0x24(r1) # replaced
    mflr    r11
    stw     r11, 0x68(r1) # stash lr too
    LOADW   r3,  debugLogEnd # prepare params for sprintf
    lwz     r4,  0x08(r1) # must shift all params up one register
    lwz     r5,  0x0C(r1)
    lwz     r6,  0x10(r1)
    lwz     r7,  0x14(r1)
    lwz     r8,  0x18(r1)
    lwz     r9,  0x1C(r1)
    lwz     r10, 0x20(r1)
    lwz     r11, 0x24(r1)
    CALL    sprintf # returns # characters written excluding null

    # update pLogEnd
    LOADWH  r4, debugLogEnd
    LOADWL2 r5, debugLogEnd, r4
    add     r5, r5, r3
    addi    r5, r5, 1 # or else it deadlocks displaying
    STOREW  r5, debugLogEnd, r4
    lwz     r5, 0x68(r1)
    mtlr    r5
    JUMP    0x80137990, r0


mainLoop: # called from main loop. r3 = mainLoop
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r4
    stw   r4,  SP_LR_SAVE(r1)
    stw   r14, SP_R14_SAVE(r1)
    stw   r15, SP_R15_SAVE(r1)
    stw   r16, SP_R16_SAVE(r1)
    mr    r14, r3

    # the patch that restores debug print functions also overrides
    # this variable, and won't affect our bar drawing,
    # so manually check it before doing things.
    LOADB r4, enableDebugText
    cmpwi r4, 0
    beq   .end

    # don't make debug text flash with HUD text
    # XXX why doesn't this work?
    li      r3, 255
    li      r4, 255
    li      r5, 255
    li      r6, 255
    CALL    gameTextSetColor

    LOAD  r18, 0x803a89b0 # HUD textures

    # draw full bar at reduced opacity to show range
    lfs     f1, (.cpuBarX - mainLoop)(r14)
    lfs     f2, (.cpuBarY - mainLoop)(r14)
    li      r6, 200    # width
    li      r3, 11     # texture (box left side, but we flip it)
    li      r4, 0x7F   # opacity
    li      r7, 30     # height
    bl      .drawBarWithOpacityAndHeight

    # display CPU usage
    # XXX this isn't a very good indicator. it's frame time, which is
    # always going to be at least 16.7ms because it includes idle time.
    # is there a proper CPU usage variable anywhere?
    LOADWH  r4, msecsThisFrame
    LOADFL2 f1, msecsThisFrame, r4
    lfs     f2, (.timeDeltaScale - mainLoop)(r14)
    fmuls   f2, f2, f1
    fctiwz  f2, f2
    stfd    f2, SP_FLOAT_TMP(r1)
    lwz     r6, SP_FLOAT_TMP+4(r1) # width
    lfs     f1, (.cpuBarX - mainLoop)(r14) # X
    lfs     f2, (.cpuBarY - mainLoop)(r14) # Y
    li      r3, 13     # texture
    bl      .drawBar

    # display heap stats: free bytes, free blocks
    LOAD  r16, 0x803406A0 # heap 0
    li    r15, 0 # idx
    lfs   f31, (.heapBarY - mainLoop)(r14)
    lfs   f30, (.heapBarHeight - mainLoop)(r14)
.nextHeap:
    lwz   r4, 0x0C(r16) # total bytes
    #lwz   r5, 0x00(r16) # total blocks
    lwz   r6, 0x10(r16) # used bytes
    sub   r4, r4, r6    # free bytes
    #mulli r6, r6, 200
    #divwu r6, r6, r4 # used bytes percent * 2
    #lwz   r6, 0x04(r16) # used blocks
    ##sub   r5, r5, r6    # free blocks
    #mulli r6, r6, 100
    #divwu r5, r6, r5 # used blocks percent

    srwi    r6, r4, 14 # free 16KByte blocks

    lfs     f1, (.cpuBarX - mainLoop)(r14) # X
    fmr     f2, f31 # Y
    fadds   f31, f31, f30
    li      r3, 13     # texture
    bl      .drawBar
    # XXX blocks?

    addi  r16, r16, 0x14 # heap entry size
    addi  r15, r15, 1
    cmpwi r15, 4
    bne   .nextHeap

    # get player object
    LOADW r16, pPlayer
    cmpwi r16, 0
    beq   .noPlayer

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

    addi r3, r14, (.fmt_playerCoords - mainLoop)@l
    mr   r7, r16 # player*
    CALL debugPrintf

    # display map cell
    addi r3, r14, (.fmt_mapCoords - mainLoop)@l
    LOADWH  r9, mapCoords
    #LOADWL2 r4, mapCoords,    r9
    #LOADWL2 r5, mapCoords+ 4, r9
    LOADWL2 r4, mapCoords+ 8, r9
    LOADWL2 r5, mapCoords+12, r9
    LOADBL2 r6, curMapLayer,  r9
    extsb   r6, r6
    LOADWL2 r7, curMapId,     r9
    #ori     r8, r10, 0 # r9 = nearest object
    #ori     r9, r17, 0 # r10 = name
    CALL debugPrintf

    # display player state
    #addi r3, r14, (.fmt_playerState - mainLoop)@l
    #lwz  r9, 0x00B8(r16) # get animState
    ##lfs  f1, 0x0098(r16) # get anim timer
    ##lfs  f2, 0x0814(r9)  # get anim val
    #lbz  r4, 0x0275(r9)  # get state ID
    #lwz  r5, 0x03F0(r9)  # get flags
    #lhz  r6, 0x00A0(r16) # get anim ID
    ## magic required to make floats print correctly
    ## no idea what this does
    #creqv 4*cr1+eq,4*cr1+eq,4*cr1+eq
    #CALL debugPrintf


.noPlayer:
    # get camera object
    LOADW r16, pCamera
    cmpwi r16, 0
    beq   .noCamera

    # display camera coords
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

    addi r3, r14, (.fmt_cameraCoords - mainLoop)@l
    CALL debugPrintf

    # display game state info
    addi  r3, r14, .fmt_gameState - mainLoop
    LOADW r4, 0x803dcb84 # numObjects
    LOADW r5, 0x803dcde8 # game state flags
    CALL  debugPrintf

    # display GameText info
    # XXX why is this sometimes 0 when there is a text?
    LOADHA r4, curGameText
    cmpwi  r4, 0
    blt    .noText
    LOADW  r5, 0x803a9444 # cur phrase
    addi   r3, r14, .fmt_textState - mainLoop
    CALL   debugPrintf

.noText:

    # display sequence info (XXX incomplete/wrong)
    LOADWH  r9, curSeqNo
    LOADBL2 r4, curSeqNo, r9
    cmpwi   r4, 0
    beq     .noSeq

    LOADBL2 r5, seqPos,    r9
    LOADWL2 r6, seqLength, r9
    LOADWL2 r7, curSeqObj, r9
    addi    r3, r14, .fmt_seqState - mainLoop
    CALL    debugPrintf

.noSeq:

    # display nearby object
    # or really, the current "target" object (that the heart, A icon,
    # etc is over)
    LOADW r17, pCamera
    cmpwi r17, 0
    beq   .noObject

    lwz   r4, 0x124(r17)
    cmpwi r4, 0
    beq   .noObject
    lwz   r5,  0x50(r4) # get file ptr
    cmpwi r5,  0
    beq   .noObject

    # manually copy the name since it's not terminated
    addi    r3, r1, SP_OBJ_NAME
    addi    r4, r5, 0x91 # name
    li      r5, 11
    CALL    strncpy
    li      r4,  0
    stb     r4,  (SP_OBJ_NAME+12)(r1) # strncpy doesn't do this if src is long
    lwz     r4,  0x124(r17) # obj ptr
    addi    r5,  r1, SP_OBJ_NAME
    addi    r3,  r14, (.fmt_nearObj - mainLoop)@l
    CALL    debugPrintf
.noObject:
.end:
    lwz  r14, SP_R14_SAVE(r1)
    lwz  r15, SP_R15_SAVE(r1)
    lwz  r16, SP_R16_SAVE(r1)
    lwz  r3,  SP_LR_SAVE(r1)
    mtlr r3 # restore LR
    addi r1, r1, STACK_SIZE # restore stack ptr
    blr

.drawBar:
    li      r4, 0xFF    # opacity
    li      r7, 5       # height
.drawBarWithOpacityAndHeight:
    slwi    r3, r3, 2   # texture ID * 4
    lwzx    r3, r3, r18 # Texture*
    li      r5, 0x0100  # scale
    li      r8, 1       # flags: H flip (so border is at right)
    mflr    r29
    CALL    0x8007681c  # draw scaled texture
    mtlr    r29
    blr

#.timeDeltaScale: .float 6.25 # 100 / 16
.timeDeltaScale: .float 12.0 # 200 / 16.666666...
#.timeDeltaScale: .float 18.75 # 300 / 16
.cpuBarX:        .float 1
.cpuBarY:        .float 463
.heapBarY:       .float 466
.heapBarHeight:  .float 3
.floatMagic: .int 0x43300000,0x80000000
.fmt_playerCoords: .string "P:\x84%6d %6d %6d %08X\x83 "
.fmt_mapCoords:    .string "M:\x84%3d %3d L%d %02X\x83\n"
#.fmt_playerState:  .string "S:\x84%02X %08X\x83 A:\x84%04X\x83\n"
.fmt_nearObj:      .string "O:\x84%08X %s\x83\n"
.fmt_cameraCoords: .string "C:\x84%6d %6d %6d\x83 "
.fmt_gameState:    .string "Obj\x84%3d\x83 G:\x84%08X\x83\n"
.fmt_textState:    .string "TEXT %04X %08X\n"
.fmt_seqState:     .string "SEQ %02X pos %X/%X obj %08X\n"
bootMsg: .string "Mem size %08X (sim %08X), ARAM %08X, monitor %08X @ %08X, arena %08X - %08X"
