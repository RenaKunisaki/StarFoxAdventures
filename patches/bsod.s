# bsod patch:
# shows more helpful info on BSOD screen.
.text
.include "common.s"
.include "globals.s"
.set BSOD_MAX_PAGE,6

# idea: press Z to save to slot 3 or some other file on the memory card,
# so you can hopefully not lose progress.

# define patches
patchList:
    PATCH_ID        "BSOD   " # must be 7 chars
    #PATCH_BL        0x80137f3c, onBSOD
    PATCH_B         0x80137e0c, onBSOD
    PATCH_BYTE      0x8031d264, 0x08 # remove excess spacing from stack trace
    PATCH_BYTE      0x8031d269, 0x20 # remove excess spacing from stack trace
    PATCH_END       PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE, 0x100 # how much to reserve
    .set SP_LR_SAVE, 0x104
    .set SP_FLOAT_TMP,0x20 # temporary storage for float conversion (8 bytes)
    .set SP_GPR_SAVE, 0x28

entry: # called as soon as our patch is loaded.
    blr # nothing to do here


onBSOD:
    # replace BSOD handler function
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r9
    stw     r9,  SP_LR_SAVE(r1)

    bl .getpc
    .getpc:
        mflr r14

    #CALL    0x8006c830 # updateReflectionTexture

    # copied game code
    CALL    OSDisableInterrupts
    mr      r15, r3 # prev MSR

    # these buffers are needed for debug print
    lis     r4,  0x803e
    lwz     r3,  (0x803dccec)@l(r4) # pFrameBuffer1
    stw     r3,  (0x803dda30)@l(r4) # debugFrameBuffer1
    lwz     r3,  (0x803dcce8)@l(r4) # pFrameBuffer2
    stw     r3,  (0x803dda2c)@l(r4) # debugFrameBuffer2

    # disable some video functions so we can use the framebuffer
    li      r3,  0
    CALL    0x8024c168 # videoSetBufferSwapCb
    li      r3,  0
    CALL    0x8024c1ac # setGpuErrorHandler
    li      r3,  0
    CALL    0x80256460 # videoSetCallback_80256460
    CALL    0x80258330 # gxWaitFn_80258330
    mr      r3,  r15
    CALL    OSRestoreInterrupts


bsodLoop: # never ends
    lwz     r15, -0x57b0(r13) # get frame buffer
    lwz     r16, -0x57a4(r13) # bsodRegs

    lbz     r3,  (bsodPage - .getpc)(r14)
    slwi    r3,  r3,  1
    addi    r3,  r3,  pageFuncs - .getpc
    lhzx    r3,  r3,  r14 # get func offset
    add     r3,  r3,  r14 # get func ptr
    mtspr   CTR, r3
    bctrl

end:
    # draw page number
    li      r3,  620 # X
    li      r4,  438 # Y
    addi    r5,  r14, s_page - .getpc
    lbz     r6,  (bsodPage - .getpc)(r14)
    CALL    debugPrintfxy

    mr      r3,  r15 # framebuffer
    LOAD    r4,  0x96000
    CALL    dCacheStore

    # check input
    LOAD    r3,  controller1state
    CALL    0x8024e864 # padReadControllers

    # swap frame buffers
    lwz     r3,  -0x57b0(r13)
    lwz     r4,  -0x57b4(r13)
    stw     r4,  -0x57b0(r13)
    stw     r3,  -0x57b4(r13)

    CALL    VISetNextFrameBuffer
    CALL    0x8024d554 # unknown
    CALL    VIWaitForRetrace

    # padReadControllers seems to just return all zeros if done too often,
    # so the only way to avoid flashing through all pages on every button
    # press is to only poll every so often.
    lbz     r3,  (padDelay - .getpc)(r14)
    cmpwi   r3,  0
    bne     .delayRead

    LOADH   r3,  controller1state
    andi.   r0,  r3,  PAD_BUTTON_A
    bne     .nextPage
    andi.   r0,  r3,  PAD_BUTTON_B
    bne     .prevPage
    andi.   r0,  r3,  PAD_BUTTON_MENU
    bne     .reset

    b       bsodLoop
    #lwz     r9,  SP_LR_SAVE(r1)
    #mtlr    r9 # restore LR
    #lmw     r3,  SP_GPR_SAVE(r1)
    #addi    r1,  r1,  STACK_SIZE # restore stack ptr
    #blr

.nextPage:
    lbz     r3,  (bsodPage - .getpc)(r14)
    addi    r3,  r3,  1
    cmpwi   r3,  BSOD_MAX_PAGE
    ble     .setPage
    li      r3,  0

.setPage:
    stb     r3,  (bsodPage - .getpc)(r14)
    li      r3,  120
    stb     r3,  (padDelay - .getpc)(r14)
    b       bsodLoop

.prevPage:
    lbz     r3,  (bsodPage - .getpc)(r14)
    cmpwi   r3,  0
    beq     .wrapLastPage
    subi    r3,  r3,  1
    b       .setPage
.wrapLastPage:
    li      r3,  BSOD_MAX_PAGE
    b       .setPage

.delayRead:
    subi    r3,  r3,  1
    stb     r3,  (padDelay - .getpc)(r14)
    b       bsodLoop

.reset:
    # this seems to fail more often than not
    li      r3,  1
    LOADWH  r4,  0x803dca3e
    STOREB  r3,  0x803dca3e, r4 # shouldResetNextFrame
    STOREB  r3,  0x803dcca6, r4 # shouldReset
    li      r3,  0
    STOREB  r3,  0x803dc951, r4 # inhibitReset
    li      r3,  4 # GAME_STATE_RESETNOW
    STOREB  r3,  0x803dca3d, r4 # gameState
    lwz     r3,  -0x57b0(r13)
    CALL    VISetNextFrameBuffer
    CALL    0x8024d554 # unknown
    CALL    VIWaitForRetrace
    JUMP    0x80021364, r0

clearFrameBuffer: # expects frame buffer in r15
    # fill frame buffer
    # framebuffer is YUYV
    # Y=29.07, U=255.5, V=107.26595 should be dark blue
    # but I have no idea so black it is
    LOAD    r3,  0x10801080 # black
    li      r4,  0
    LOAD    r5,  (640 * 480 * 2)
    .fillNext:
    stwx    r3,  r15, r4
    addi    r4,  r4,  4
    cmpw    r4,  r5
    blt     .fillNext
    blr


drawPage0: # general info
    mflr    r29
    bl      clearFrameBuffer

    li      r3,  16 # X
    li      r4,  30 # Y
    addi    r5,  r14, s_errMain - .getpc
    CALL    debugPrintfxy

    # print error code and PC
    lwz     r6,  0x0198(r16) # PC
    lhz     r7,  -0x57a0(r13) #bsodErrorCode
    slwi    r8,  r7,  1 # keep error code even if invalid
    cmpwi   r7,  0xF
    ble     .errCodeValid
    li      r8,  0 # but don't keep string
.errCodeValid:
    addi    r8,  r8,  errStrs - .getpc
    lhzx    r8,  r8,  r14 # get offset of string ptr
    lwzx    r8,  r8,  r14 # get string ptr
    li      r3,  26 # X
    li      r4,  85 # Y
    addi    r5,  r14, s_errorPC - .getpc
    CALL    debugPrintfxy
    #b       end

    LOADW   r18, pPlayer
    cmpwi   r18, 0
    beq     .noPlayer

    # display player coords
    # debugPrintf doesn't support eg '%+7.2f' so we'll just convert
    # to int to get rid of excess digits
    lfs     f1,  0x0C(r18)
    fctiwz  f2,  f1
    stfd    f2,  SP_FLOAT_TMP(r1)
    lwz     r7,  SP_FLOAT_TMP+4(r1)

    lfs     f1,  0x10(r18)
    fctiwz  f2,  f1
    stfd    f2,  SP_FLOAT_TMP(r1)
    lwz     r8,  SP_FLOAT_TMP+4(r1)

    lfs     f1,  0x14(r18)
    fctiwz  f2,  f1
    stfd    f2,  SP_FLOAT_TMP(r1)
    lwz     r9,  SP_FLOAT_TMP+4(r1)

    li      r3,  26 # X
    li      r4,  96 # Y
    addi    r5,  r14, s_playerPos - .getpc
    mr      r6,  r18
    CALL    debugPrintfxy

.noPlayer:
    LOADW   r6,  cameraMode
    LOADB   r7,  curMapAct
    LOADW   r8,  numLoadedObjs
    li      r3,   26 # X
    li      r4,  107 # Y
    addi    r5,  r14, s_info1 - .getpc
    CALL    debugPrintfxy

    LOAD    r3,  loadedFileMapIds
    lhz     r6,  (MODELS_BIN *2)(r3)
    lhz     r7,  (MODELS_BIN2*2)(r3)
    li      r3,  150 # X
    li      r4,  107 # Y
    addi    r5,  r14, s_info2 - .getpc
    CALL    debugPrintfxy

    # show GPRs
    li      r3,   26 # X
    li      r4,  122 # Y
    LOAD    r5,  0x8031d270 # "General purpose registers"
    CALL    debugPrintfxy

    li      r17,  8  # count
    mr      r18, r16 # regs
    li      r19, 133 # Y

.nextGPR:
    li      r3,  26  # X
    mr      r4,  r19
    addi    r5,  r14, s_08X_4 - .getpc
    lwz     r6,  0x00(r18)
    lwz     r7,  0x04(r18)
    lwz     r8,  0x08(r18)
    lwz     r9,  0x0C(r18)
    CALL    debugPrintfxy
    addi    r18, r18, 0x10 # next regs
    addi    r19, r19, 11 # next line
    subi    r17, r17, 1
    cmpwi   r17, 0
    bgt     .nextGPR

    # draw stack dump
    li      r3,   26 # X
    li      r4,  228 # Y
    addi    r5,  r14, s_stackDump - .getpc
    CALL    debugPrintfxy

    lwz     r18, 0x04(r16) # sp
    srwi    r3,  r18, 20
    cmpwi   r3,  0x800
    blt     .doneStack
    cmpwi   r3,  0x818
    bge     .doneStack
    li      r17, 16 # count
    li      r19, 239 # Y
.nextStack:
    li      r3,  26 # X
    mr      r4,  r19
    addi    r5,  r14, s_08X_4 - .getpc
    lwz     r6,  -0x00(r18)
    lwz     r7,  -0x04(r18)
    lwz     r8,  -0x08(r18)
    lwz     r9,  -0x0C(r18)
    CALL    debugPrintfxy
    addi    r18, r18, -0x10 # next
    addi    r19, r19, 11 # next line
    subi    r17, r17, 1
    cmpwi   r17, 0
    bgt     .nextStack

.doneStack:
    # show stack trace
    li      r3,  440 # X
    li      r4,  122 # Y
    LOAD    r5,  0x8031d24A # "trace"
    CALL    debugPrintfxy

    lwz     r18, 0x04(r16) # get sp
    li      r19, 133 # Y
    li      r17,   0 # count
.nextTrace:
    srwi    r3,  r18, 20
    cmpwi   r3,  0x800
    blt     .doneTrace
    cmpwi   r3,  0x818
    bge     .doneTrace
    cmpwi   r17, 32
    bge     .doneTrace

    li      r3,  440 # X
    mr      r4,  r19
    addi    r5,  r14, s_08X_1 - .getpc
    lwz     r6,  0x04(r18)
    lwz     r18, 0x00(r18) # next frame
    CALL    debugPrintfxy
    addi    r19, r19, 11
    b       .nextTrace

.doneTrace:
    # show instructions
    li      r3,   26 # X
    li      r4,  430 # Y
    addi    r5,  r14, s_instrs - .getpc
    CALL    debugPrintfxy

.donePage:
    mtlr    r29
    blr


drawPage1: # memory info
    mflr    r29
    bl      clearFrameBuffer

    # copy reflection texture to framebuffer
    # unfortunately it seems to be just nonsense
#    LOADW   r18, 0x803dcf7c
#    addi    r18, r18, 0x60
#    li      r16, 0 # row
#    LOAD    r17, ((640 * 96) + 320) * 2 # ((screenW * y) + x)
#    add     r17, r17, r15
#.copyNextRow:
#    mr      r3,  r17 # dest
#    mr      r4,  r18 # src
#    li      r5,  320 * 2 # len
#    CALL    memcpy
#    addi    r18, r18, 320*2
#    addi    r17, r17, 640*2
#    addi    r16, r16, 1
#    cmpwi   r16, 240
#    blt     .copyNextRow

    # heap title
    li      r3,   26 # X
    li      r4,   20 # Y
    addi    r5,  r14, s_heapTitle - .getpc
    CALL    debugPrintfxy

    # show heap stats
    LOAD    r18, 0x803406A0 # heap 0
    li      r15,  0 # idx
    li      r17, 31 # text Y pos
    .nextHeap:
    li      r3,  26 # X
    mr      r4,  r17
    addi    r5,  r14, s_heap - .getpc
    mr      r6,  r18
    lwz     r7,  0x10(r18) # used bytes
    lwz     r8,  0x0C(r18) # total bytes
    lwz     r9,  0x04(r18) # used blocks
    lwz     r10, 0x00(r18) # total blocks
    CALL    debugPrintfxy

    addi    r18, r18, 0x14 # heap entry size
    addi    r17, r17, 11
    addi    r15, r15, 1
    cmpwi   r15, 4
    bne     .nextHeap

    # show alloc fail log
    LOADW   r3,  PATCH_STATE_PTR
    addi    r18, r3,  ALLOC_FAIL_LOG
    lbz     r6,  ALLOC_FAIL_POS(r3)
    li      r3,   26 # X
    li      r4,   86 # Y
    addi    r5,  r14, s_allocFail - .getpc
    CALL    debugPrintfxy

    li      r15,  0 # count
    li      r17, 97 # Y
.nextAllocFail:
    li      r3,  26 # X
    mr      r4,  r17
    addi    r5,  r14, s_08X_2 - .getpc
    lwz     r6,  0x00(r18) # address/tag
    lwz     r7,  0x04(r18) # size
    CALL    debugPrintfxy
    addi    r18, r18, 8
    addi    r17, r17, 11
    addi    r15, r15, 1
    cmpwi   r15, ALLOC_FAIL_MAX
    blt     .nextAllocFail

    b       .donePage


drawPage2: # player pos log
    mflr    r29
    bl      clearFrameBuffer

    li      r3,   26 # X
    li      r4,   20 # Y
    addi    r5,  r14, s_posLogTitle - .getpc
    CALL    debugPrintfxy

    # unused log of previous player coordinates
    LOAD     r16, 0x8033bfb8
    li       r17,  0 # count
    li       r18, 31 # Y pos
    li       r19, 26 # X pos
.nextPosLog:
    lfs     f1,  0x00(r16)
    fctiwz  f2,  f1
    stfd    f2,  SP_FLOAT_TMP(r1)
    lwz     r6,  SP_FLOAT_TMP+4(r1)

    lfs     f1,  0x04(r16)
    fctiwz  f2,  f1
    stfd    f2,  SP_FLOAT_TMP(r1)
    lwz     r7,  SP_FLOAT_TMP+4(r1)

    lfs     f1,  0x08(r16)
    fctiwz  f2,  f1
    stfd    f2,  SP_FLOAT_TMP(r1)
    lwz     r8,  SP_FLOAT_TMP+4(r1)

    lwz     r9,  0x0C(r16)
    mr      r3,  r19
    mr      r4,  r18
    addi    r5,  r14, s_posLog - .getpc
    CALL    debugPrintfxy

    addi    r16, r16, 0x10
    addi    r18, r18, 11
    addi    r17, r17, 1
    cmpwi   r17, 30
    beq     .posLogNextCol
    cmpwi   r17, 60
    blt     .nextPosLog

    b       .donePage
.posLogNextCol:
    li      r18, 31
    li      r19, 340
    b       .nextPosLog


drawPage3: # file table part 1
    mflr    r29
    bl      clearFrameBuffer

    LOAD    r16, loadedFileMapIds
    LOAD    r17, dataFileBuffers
    LOAD    r18, dataFileSize
    li      r19,  0 # idx
.drawFileTable:
    li      r20, 31 # Y pos
    li      r21, 26 # X pos

    li      r3,   26 # X
    li      r4,   20 # Y
    addi    r5,  r14, s_filesTitle - .getpc
    CALL    debugPrintfxy

.nextFile:
    mr      r3,  r21
    mr      r4,  r20
    addi    r5,  r14, s_file - .getpc
    mr      r6,  r19
    lhz     r7,  0(r16) # map ID
    lwz     r8,  0(r17) # address
    lwz     r9,  0(r18) # size
    CALL    debugPrintfxy

    addi    r16, r16, 2
    addi    r17, r17, 4
    addi    r18, r18, 4
    addi    r19, r19, 1
    addi    r20, r20, 11
    cmpwi   r19, 88
    bge     .donePage
    cmpwi   r19, 38
    beq     .fileTableNextCol
    cmpwi   r20, 449
    blt     .nextFile
    b       .donePage
.fileTableNextCol:
    li      r20, 31
    li      r21, 320
    b       .nextFile


drawPage4: # file table part 2
    mflr    r29
    bl      clearFrameBuffer

    LOAD    r16, loadedFileMapIds + (76*2)
    LOAD    r17, dataFileBuffers  + (76*4)
    LOAD    r18, dataFileSize     + (76*4)
    li      r19, 76 # idx
    b       .drawFileTable


drawHexDump:
    # r19: Y pos
    # r20: number of bytes
    # r21: data
    li      r18, 0 # byte index
    mflr    r22
.nextRow:
    li      r3,   26 # X
    mr      r4,  r19
    addi    r5,  r14, s_08X_4 - .getpc
    lwz     r6,  0x00(r21)
    lwz     r7,  0x04(r21)
    lwz     r8,  0x08(r21)
    lwz     r9,  0x0C(r21)
    CALL    debugPrintfxy
    addi    r19, r19, 11
    addi    r21, r21, 16
    addi    r18, r18, 16
    cmpw    r18, r20
    blt     .nextRow
    mtlr    r22
    blr


drawPage5: # bit tables 0, 1, 3
    mflr    r29
    bl      clearFrameBuffer

    li      r3,   26 # X
    li      r4,   20 # Y
    addi    r5,  r14, s_bitTable - .getpc
    CALL    debugPrintfxy

    li      r19,  31 # Y
    li      r20, 128 # number of bytes
    LOAD    r21, 0x803a4198 # table 0
    bl      drawHexDump

    li      r19, 130 # Y
    li      r20, 0x74 # number of bytes
    LOADW   r21, pLastSavedGame
    addi    r21, r21, 0x0564 # table 1
    bl      drawHexDump

    li      r19, 229 # Y
    li      r20, 0xAC # number of bytes
    LOADW   r21, pLastSavedGame
    addi    r21, r21, 0x05D8 # table 3
    bl      drawHexDump
    b       .donePage

drawPage6: # bit table 2, too big for the other page
    mflr    r29
    bl      clearFrameBuffer

    li      r3,   26 # X
    li      r4,   20 # Y
    addi    r5,  r14, s_bitTable - .getpc
    CALL    debugPrintfxy

    li      r19,  31 # Y
    li      r20, 0x144 # number of bytes
    LOADW   r21, pLastSavedGame
    addi    r21, r21, 0x24 # table 2
    bl      drawHexDump

    b       .donePage


# arrange items by size to avoid the need for padding.
ps_unknownError: .int 0x8031d234 # "unknown error"
ps_error1: .int 0x8031d1d0 # "machine check"
ps_error2: .int 0x803dbc18 # "DSI"
ps_error3: .int 0x803dbc1c # "ISI"
ps_error5: .int 0x8031d1e0 # "alignment"
ps_errorB: .int 0x8031d1ec # "performance monitor"
ps_errorD: .int 0x8031d200 # "system management interrupt"
ps_errorF: .int 0x8031d21c # "memory protection error"

errStrs:
    .short ps_unknownError - .getpc # 0
    .short ps_error1       - .getpc # 1
    .short ps_error2       - .getpc # 2
    .short ps_error3       - .getpc # 3
    .short ps_unknownError - .getpc # 4
    .short ps_error5       - .getpc # 5
    .short ps_unknownError - .getpc # 6
    .short ps_unknownError - .getpc # 7
    .short ps_unknownError - .getpc # 8
    .short ps_unknownError - .getpc # 9
    .short ps_unknownError - .getpc # A
    .short ps_errorB       - .getpc # B
    .short ps_unknownError - .getpc # C
    .short ps_errorD       - .getpc # D
    .short ps_unknownError - .getpc # E
    .short ps_errorF       - .getpc # F

pageFuncs:
    .short drawPage0 - .getpc
    .short drawPage1 - .getpc
    .short drawPage2 - .getpc
    .short drawPage3 - .getpc
    .short drawPage4 - .getpc
    .short drawPage5 - .getpc
    .short drawPage6 - .getpc

bsodPage: .byte 0
padDelay: .byte 180

s_errMain:
    .ascii  "       --- FLAGRANT SYSTEM ERROR ---\n"
    .ascii  "Please send photos/screenshots of each page to\n"
    .ascii  "       hyperhacker at gmail dot com\n"
    .string "    along with your save file if possible."
s_instrs:     .string "A:NEXT B:PREV START:RESET"
s_page:       .string "P%d"
s_errorPC:    .string "PC %08X ERR %X %s"
s_playerPos:  .string "P %08X %6d %6d %6d"
s_info1:      .string "%02X%02X %04X" # camera mode, cur map act, num objects
s_info2:      .string "%04X %04X" # cur map, submap
s_08X_1:      .string "%08X"
s_08X_2:      .string "%08X %08X"
s_08X_4:      .string "%08X %08X %08X %08X"
s_stackDump:  .string "STACK DUMP"
s_heapTitle:  .string "HEAP ADR/B USED/B TOTL/BLKU/BLKT"
s_heap:       .string "%08X %06X %06X %04X %04X"
s_allocFail:  .string "ALLOC FAIL LOG %02X"
s_posLogTitle:.string "PLAYER POS LOG"
s_posLog:     .string "%6d %6d %6d %06X"
s_filesTitle: .string "FILE TABLE"
s_file:       .string "%02X %04X %08X %08X"
s_bitTable:   .string "BIT TABLES"

# 803dcf7c pReflectionTexture1
# 803dcfdc pReflectionTexture2
