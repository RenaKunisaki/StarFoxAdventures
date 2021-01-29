# debugprint patch:
# 1) patches into main loop to display some info on screen
# 2) moves debug print to very edges of screen
# 3) draws bars in bottom left corner showing CPU time and memory usage
.text
.include "common.s"
.include "globals.s"
.set REDIRECT_TO_CONSOLE,0 # send on-screen prints to console instead

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
.if REDIRECT_TO_CONSOLE
    PATCH_B    0x80137948, hook_logPrintf # patch very beginning
.else
    PATCH_B    0x8013798C, hook_logPrintf
.endif
    PATCH_BL   0x80020c74, hook_debugPrintDraw

    # patch debug print to move to the very edge of the screen.
    PATCH_HWORD 0x8013761A, 0
    PATCH_HWORD 0x8013762E, 0
    PATCH_HWORD 0x8013764A, 0
    #PATCH_HWORD 0x8013765E, 0 # this causes a glitch at bottom when fading
    PATCH_WORD  0x80137830, 0x38000000 # these two prevent fade glitch
    PATCH_WORD  0x80137688, 0x38000000

    # make fixed width more reasonable
    # we can even do 5 here, but then the letter B gets cut off
    # and looks like 8.
    PATCH_BYTE  0x80137317, 6

    # make text a little smaller
    PATCH_HWORD 0x803e23b8, 0x3FA0
    #PATCH_HWORD 0x803dd9dc, 0x3FA0

    PATCH_WORD 0x80137cf4, 0x3BFF000C # make debugPrintfxy text smaller

    # patch chapter select to just Z button
    PATCH_WORD 0x80119D90, 0x60000000

    # patch BSOD thread to show error even if debug text is off
    PATCH_BL   0x80137E08,  hook_bsod

    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,  0x40 # how much to reserve
    .set SP_LR_SAVE,  0x44
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

    # hold Z to enable debug text at boot
    # disabled because we do this in startmsg.s instead.
#    LOADWH  r9,  controllerStates
#    LOADHL2 r6,  controllerStates, r9 # buttons
#    andi.   r6,  r6, PAD_BUTTON_Z
#    beq     .notZheld
#    LOADWH  r4,  enableDebugText
#    li      r3,  1
#    STOREB  r3,  enableDebugText, r4
#.notZheld:
    lwz  r14, SP_R14_SAVE(r1)
    lwz  r3,  SP_LR_SAVE(r1)
    mtlr r3   # restore LR
    addi r1,  r1, STACK_SIZE # restore stack ptr
    blr


hook_logPrintf: # hooked beginning of logPrintf
.if REDIRECT_TO_CONSOLE
    JUMP    OSReport, r0
.else
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
.endif


hook_debugPrintDraw: # hooked call to debugPrintDraw
    # param: r3=unused
    # here we just ensure that the text doesn't show up when
    # it isn't supposed to.
    LOADB  r4, enableDebugText
    cmpwi  r4, 0
    beq    .hook_debugPrintDraw_off
    JUMP   debugPrintDraw, r4

.hook_debugPrintDraw_off: # debug text is off
    # clear log buffers so we don't get a huge flood
    # when we do turn debug text on
    LOAD   r3, 0x803aa018
    LOADWH r4, 0x803dbc14
    STOREW r3, 0x803dbc14, r4 # reset debug log buffer
    li     r3, 0
    STOREW r3, 0x803dd9e4, r4
    STOREH r3, 0x803dda18, r4
    STOREH r3, 0x803dda1a, r4
    # even though debug text is off, we still need to call the function,
    # or else there will be visual glitches.
    JUMP   debugPrintDraw, r4


hook_bsod: # hooked call to storeRegs24 from BSOD render
    mflr    r3
    CALL    0x802860d0 # storeRegs24
    mtlr    r3
    LOADWH  r3,  enableDebugText
    li      r28, 1
    STOREB  r28, enableDebugText, r3
    blr


mainLoop: # called from main loop. r3 = mainLoop
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r4
    stw     r4,  SP_LR_SAVE(r1)
    stw     r14, SP_R14_SAVE(r1)
    stw     r15, SP_R15_SAVE(r1)
    stw     r16, SP_R16_SAVE(r1)
    mr      r14, r3

    # the patch that restores debug print functions also overrides
    # this variable, and won't affect our bar drawing,
    # so manually check it before doing things.
    LOADB   r4,  enableDebugText
    cmpwi   r4,  0
    beq     .end

    LOADW   r19,  PATCH_STATE_PTR
    lbz     r19,  DEBUG_TEXT_FLAGS(r19)

.if REDIRECT_TO_CONSOLE # print a header message
    addi    r3,  r14, (.fmt_start - mainLoop)@l
    CALL    debugPrintf
.endif

    # don't make debug text flash with HUD text
    # XXX why doesn't this work?
    li      r3,  255
    li      r4,  255
    li      r5,  255
    li      r6,  255
    CALL    gameTextSetColor

    LOAD    r18, 0x803a89b0 # HUD textures

    # draw full bar at reduced opacity to show range
    lfs     f1,  (.cpuBarX - mainLoop)(r14)
    lfs     f2,  (.cpuBarY - mainLoop)(r14)
    li      r6,  200    # width
    li      r3,  11     # texture (box left side, but we flip it)
    li      r4,  0x7F   # opacity
    li      r7,  30     # height
    bl      .drawBarWithOpacityAndHeight

    # display CPU usage
    # XXX this isn't a very good indicator. it's frame time, which is
    # always going to be at least 16.7ms because it includes idle time.
    # is there a proper CPU usage variable anywhere?
    LOADWH  r4,  msecsThisFrame
    LOADFL2 f1,  msecsThisFrame, r4
    lfs     f2,  (.timeDeltaScale - mainLoop)(r14)
    fmuls   f2,  f2, f1
    fctiwz  f2,  f2
    stfd    f2,  SP_FLOAT_TMP(r1)
    lwz     r6,  SP_FLOAT_TMP+4(r1) # width
    lfs     f1,  (.cpuBarX - mainLoop)(r14) # X
    lfs     f2,  (.cpuBarY - mainLoop)(r14) # Y
    li      r3,  13     # texture
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
    LOADWH  r3,  mapCoords
    LOADWL2 r7,  curMapId, r3
    cmpwi   r7,  0x50
    blt     .notMap50
    LOAD    r3,  0x803a314c # map ID translation table
    lbzx    r7,  r3,  r7
.notMap50:
    LOAD    r5,  0x80311810 # objgroup flags
    slwi    r4,  r7, 1
    lhzx    r17, r5, r4 # get bit idx
    LOAD    r5,  0x80311720 # act number
    lhzx    r3,  r5, r4
    CALL    mainGetBit
    mr      r8,  r3
    mr      r3,  r17
    mr      r17, r8
    CALL    mainGetBit
    mr      r9,  r3
    mr      r8,  r17

    LOADWH  r3,  mapCoords
    LOADWL2 r4,  mapCoords+ 8, r3
    LOADWL2 r5,  mapCoords+12, r3
    LOADBL2 r6,  curMapLayer,  r3
    extsb   r6,  r6
    LOADWL2 r7,  curMapId, r3
    addi    r3,  r14, (.fmt_mapCoords - mainLoop)@l
    CALL    debugPrintf

    LOAD    r6,  loadedFileMapIds
    lbz     r4,  ((BLOCKS_BIN *2)+1)(r6)
    lbz     r5,  ((BLOCKS_BIN2*2)+1)(r6)
    addi    r3,  r14, .fmt_mapIds - mainLoop
    CALL    debugPrintf

    # %s is bugged...
#.if REDIRECT_TO_CONSOLE
#.else
#    LOADW   r3,  pCurMapInfo # map name
#    CALL    debugPrintf
#.endif

.noPlayer:
    # get camera object
    LOADW r16, pCamera
    cmpwi r16, 0
    beq   .noCamera

    # display camera coords
    lfs    f1, 0x18(r16)
    fctiwz f2,f1
    stfd   f2,SP_FLOAT_TMP(r1)
    lwz    r4,SP_FLOAT_TMP+4(r1)

    lfs    f1, 0x1C(r16)
    fctiwz f2,f1
    stfd   f2,SP_FLOAT_TMP(r1)
    lwz    r5,SP_FLOAT_TMP+4(r1)

    lfs    f1, 0x20(r16)
    fctiwz f2,f1
    stfd   f2,SP_FLOAT_TMP(r1)
    lwz    r6,SP_FLOAT_TMP+4(r1)

    LOADW  r7,  cameraMode
    LOADW  r8,  0x803dc8f0
    LOADW  r9,  0x803dc8f4

    #lfs   f2, 0x10(r16)
    #lfs   f3, 0x14(r16)
    ## magic required to make floats print correctly
    ## no idea what this does
    #creqv 4*cr1+eq,4*cr1+eq,4*cr1+eq

    addi    r3, r14, (.fmt_cameraCoords - mainLoop)@l
    CALL    debugPrintf

    # display heap stats
    LOAD    r16, 0x803406A0 # heap 0
    li      r15, 0 # idx
    li      r5,  0 # total bytes
    li      r6,  0 # total blocks
    li      r7,  0 # used bytes
    li      r8,  0 # used blocks
.printNextHeap:
    lwz     r9,  0x0C(r16) # total bytes
    add     r5,  r5,  r9
    lwz     r9,  0x00(r16) # total blocks
    add     r6,  r6,  r9
    lwz     r9,  0x10(r16) # used bytes
    add     r7,  r7,  r9
    lwz     r9,  0x04(r16) # used blocks
    add     r8,  r8,  r9
    addi    r16, r16, 0x14 # heap entry size
    addi    r15, r15, 1
    cmpwi   r15, 4
    bne     .printNextHeap

    mulli   r7,  r7,  100
    divwu   r5,  r7,  r5 # used bytes percent
    mulli   r8,  r8,  100
    divwu   r6,  r8,  r6 # used blocks percent

    # display game state info
    addi  r3, r14, .fmt_gameState - mainLoop
    LOADW r4, 0x803dcb84 # numObjects
    #LOADW r5, 0x803dcde8 # game state flags
    #LOADW r6, 0x803db700 # save status
    #LOADB r6, 0x803db424 # save status
    #LOADWH  r7, curSaveSlot
    #LOADBL2 r7, curSaveSlot, r7
    #extsb   r7, r7
    CALL  debugPrintf

    # temporary: display item vars
    #addi     r3, r14, .fmt_itemState - mainLoop
    #LOADWH   r9, 0x803dd8c0
    #LOADHL2  r4, 0x803dd8c0, r9
    #LOADHL2  r5, 0x803dd8c2, r9
    #LOADHL2  r6, 0x803dd8b8, r9
    #CALL  debugPrintf

    # display player state
    andi.   r0,  r19, DEBUG_TEXT_PLAYER_STATE
    beq     .noPlayerState
    LOADW   r16, pPlayer
    cmpwi   r16, 0
    beq     .noPlayerState
    addi    r3,  r14, (.fmt_playerState - mainLoop)@l
    lwz     r9,  0x00B8(r16) # get animState
    cmpwi   r9,  0
    beq     .noPlayerState
    lfs     f1,  0x0098(r16) # get anim timer
    lfs     f2,  0x0814(r9)  # get anim val
    lbz     r4,  0x0275(r9)  # get state ID
    lwz     r5,  0x03F0(r9)  # get flags
    lwz     r6,  0x03F4(r9)  # get flags 2
    lhz     r7,  0x00A0(r16) # get anim ID
    # magic required to make floats print correctly
    # no idea what this does
    creqv   4*cr1+eq,4*cr1+eq,4*cr1+eq
    CALL    debugPrintf
.noPlayerState:


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
.if REDIRECT_TO_CONSOLE
.else
    andi.   r0,  r19,  DEBUG_TEXT_SEQ_STATE
    beq     .noPrintSeqInfo

    LOADWH  r9,  0x803dd124
    LOADWL2 r4,  0x803dd06c, r9 # global1 and 2
    LOADBL2 r5,  0x803dd080, r9 # global3
    addi    r3,  r14, .fmt_seqState2 - mainLoop
    mflr    r20
    CALL    debugPrintf
    mtlr    r20

    # apparently only way to get correct seq info is to iterate all objs...
    LOADW   r17, loadedObjects
    lwz     r18, -4(r17) # numLoadedObjs
    slwi    r18, r18, 2
    li      r21, 0
.nextSeq:
    lwzx    r6,  r21, r17 # get obj*
    cmpwi   r6,  0
    beq     .printSeq_noObj
    lha     r4,  0xB4(r6) # curSeq
    cmpwi   r4,  -1
    bnel    printSeqInfo

.printSeq_noObj:
    addi    r21, r21, 4
    cmpw    r21, r18
    blt     .nextSeq
.endif # REDIRECT_TO_CONSOLE

.noPrintSeqInfo:
    # display heap info
    andi.   r0,  r19, DEBUG_TEXT_HEAP_STATE
    beq     .noPrintHeapDetails

    LOAD    r15, heaps
    li      r17, 0 # heap idx

.printNextHeapDetails:
    mulli   r3,  r17, 0x14
    add     r18, r3,  r15   # r18 = &Heap
    mr      r4,  r17
    lwz     r5,  0x0C(r18) # used blocks
    lwz     r6,  0x08(r18) # total blocks
    lwz     r7,  0x04(r18) # used bytes
    lwz     r8,  0x00(r18) # total bytes
    addi    r3,  r14, .fmt_heapState - mainLoop
    CALL    debugPrintf
    addi    r17, r17, 1
    cmpwi   r17, 4
    blt     .printNextHeapDetails

    # for now, show expgfx state
    li      r17, 0 # slot idx
    LOAD    r18, 0x8039bbc8
    li      r4,  0 # n slots
    li      r5,  0 # n items
.printNextExpGfx:
    lbzx    r6,  r18, r17
    cmpwi   r6,  0
    beq     .skipExpGfx
    add     r5,  r5,  r6
    addi    r4,  r4,  1
.skipExpGfx:
    addi    r17, r17, 1
    cmpwi   r17, 80
    blt     .printNextExpGfx
    addi    r3,  r14, .fmt_expgfx - mainLoop
    CALL    debugPrintf


.noPrintHeapDetails:
    # display target (that the heart, A icon, etc is over)
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
    lwz     r4,  0x0124(r17) # obj ptr
    lhz     r5,  0x0046(r4)  # defNo
    lbz     r6,  0x0138(r17) # target type
    addi    r7,  r1, SP_OBJ_NAME
    li      r8,  0
    lwz     r9,  0x4C(r4)    # seq
    cmpwi   r9,  0
    beq     .targetNoSeq
    lwz     r8,  0x14(r9)    # unique ID
.targetNoSeq:
    addi    r3,  r14, (.fmt_nearObj - mainLoop)@l
    CALL    debugPrintf
.noObject:
.end:

.if REDIRECT_TO_CONSOLE # print a footer message
    addi    r3,  r14, (.fmt_end - mainLoop)@l
    CALL    debugPrintf
.endif # REDIRECT_TO_CONSOLE

    lwz  r14, SP_R14_SAVE(r1)
    lwz  r15, SP_R15_SAVE(r1)
    lwz  r16, SP_R16_SAVE(r1)
    lwz  r3,  SP_LR_SAVE(r1)
    mtlr r3 # restore LR
    addi r1, r1, STACK_SIZE # restore stack ptr
    blr

.drawBar:
.if REDIRECT_TO_CONSOLE
.drawBarWithOpacityAndHeight:
.else
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
.endif # REDIRECT_TO_CONSOLE
    blr


.if REDIRECT_TO_CONSOLE
.else
printSeqInfo: # r6: ObjInstance*, r19: obj idx
    lwz     r3,  0x4C(r6) # get ObjDef*
    li      r4,  -1
    cmpwi   r3,  0
    beq     .printSeqInfo_noDef
    lbz     r4,  0x1F(r3)
.printSeqInfo_noDef:
    mr      r7,  r6
    LOAD    r5,  curSeqIdArray
    slwi    r0,  r4,  1
    lhax    r6,  r5,  r0
    subi    r6,  r6,  1
    lhz     r5,  0xB4(r7)
    lwz     r8,  0x50(r7) # ObjectFileStruct
    addi    r8,  r8,  0x91 # name
    addi    r3,  r14, .fmt_seqState - mainLoop
    mflr    r20
    CALL    debugPrintf
    mtlr    r20
    blr
.endif # REDIRECT_TO_CONSOLE
#           r4   r5                r6      r7
#"SEQ #\x84%02X[%02X]\x83 Obj:\x84%08X\x83 %s\n"


#.timeDeltaScale: .float 6.25 # 100 / 16
.timeDeltaScale: .float 12.0 # 200 / 16.666666...
#.timeDeltaScale: .float 18.75 # 300 / 16
.cpuBarX:        .float 1
.cpuBarY:        .float 463
.heapBarY:       .float 466
.heapBarHeight:  .float 3
.floatMagic: .int 0x43300000,0x80000000

.if REDIRECT_TO_CONSOLE
.fmt_playerCoords: .string ">> P:%6d %6d %6d %08X"
.fmt_mapCoords:    .string ">> M:%3d,%3d,%d #%02X S%X %08X"
.fmt_playerState:  .string ">> S:%02X %08X %08X A:%04X %f %f"
.fmt_cameraCoords: .string ">> C:%6d %6d %6d M%02X %02X %02X"
#.fmt_gameState:    .string "Obj\x84%3d\x83 G:\x84%08X\x83 S:%X %d\n"
.fmt_gameState:    .string ">> O:%3d M:%3d/%3d"
#.fmt_itemState:    .string "I:\x84%04X %04X %04X\x83\n"
.fmt_nearObj:      .string ">> T:%08X %04X %X %s ID:%06X"
.fmt_textState:    .string ">> X:%04X %08X"
.fmt_seqState:     .string ">> Q:%02X %08X %s"
.fmt_seqState2:    .string ">> G:%08X %02X"
.fmt_start:        .string ">> START"
.fmt_end:          .string ">> END"

.else

.fmt_playerCoords: .string "P:\x84%6d %6d %6d %08X\x83 "
.fmt_mapCoords:    .string "M:\x84%3d,%3d,%d #%02X T%X %08X\x83 "
.fmt_mapIds:       .string "L \x84%02X%02X\x83"
.fmt_playerState:  .string "S:\x84%02X %08X %08X\x83 A:\x84%04X %f %f\x83\n"
.fmt_cameraCoords: .string "\nC:\x84%6d %6d %6d\x83 M\x84%02X %02X %02X\x83 "
#.fmt_gameState:    .string "Obj\x84%3d\x83 G:\x84%08X\x83 S:%X %d\n"
.fmt_gameState:    .string "Obj\x84%3d\x83 Mem \x84%3d/%3d\x83\n"
#.fmt_itemState:    .string "I:\x84%04X %04X %04X\x83\n"
.fmt_nearObj:      .string "Target:\x84%08X %04X %X %s \x83ID:\x84%06X\x83\n"
.fmt_textState:    .string "TEXT %04X %08X\n"
.fmt_seqState:     .string "SEQ #\x84%02X[%02X] %04X\x83 Obj:\x84%08X\x83 %s\n"
.fmt_seqState2:    .string "SG %08X %02X\n"
.fmt_heapState:    .string "HEAP \x84%d\x83 Blk \x84%04X/%04X\x83 Byt \x84%08X/%08X\x83\n"
.fmt_expgfx:       .string "EXPGFX \x84%2d\x83/80 \x84%4d/2000\n"
.endif

bootMsg: .string "Mem size %08X (sim %08X), ARAM %08X, monitor %08X @ %08X, arena %08X - %08X"
