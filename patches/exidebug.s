# exidebug patch:
# implement custom EXI debug handler.
# TODO: make it not conflict with Gecko handler.
.text
.include "common.s"
.include "globals.s"

.set CMD_MISC,       0x00
.set CMD_READ_MEM,   0x01
.set CMD_WRITE_MEM,  0x02
.set CMD_READ_REGS,  0x03
.set CMD_WRITE_REGS, 0x04
.set CMD_FLUSH_CACHE,0x05
.set CMD_COPY_MEM,   0x06
.set CMD_FILL_MEM,   0x07
.set CMD_SEARCH_MEM, 0x08
.set CMD_CALL_FUNC,  0x09
.set CMD_EXEC_CODE,  0x0A
.set CMD_BREAKPOINT, 0x0B
.set NUM_CMDS,       0x0C

# define patches
patchList:
    PATCH_ID        "exidbug" # must be 7 chars
    #PATCH_MAIN_LOOP mainLoop
    PATCH_B         0x80243fe4, onExiInterrupt
    PATCH_END       PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE, 0x100 # how much to reserve
    .set SP_LR_SAVE, 0x104
    .set SP_FLOAT_TMP,0x20 # temporary storage for float conversion (8 bytes)
    .set SP_GPR_SAVE, 0x28

entry: # called as soon as our patch is loaded.
    blr # nothing to do here

onExiInterrupt:
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r9
    stw     r9,  SP_LR_SAVE(r1)

    LOADWH  r15, IO_BASE
    lwz     r3,  (EXI_IO_BASE+EXI1CSR)(r15)
    andi.   r0,  r3,  EXI_CSR_EXINT
    beq     .onExiInterrupt_skip # not our interrupt

    bl .onExiInterrupt_getpc
    .onExiInterrupt_getpc: mflr r14

    # r14: pc base
    # r15: IO_BASE
    # r30: previous CSR

    # get the data
    lwz     r30, (EXI_IO_BASE+EXI1CSR)(r15)
    li      r4,  EXI_CSR_CS2 | EXI_CSR_32MHZ
    stw     r4,  (EXI_IO_BASE+EXI1CSR)(r15)
    bl      exiRead16
    # received data is ccnnxxxx
    # cc=command, nn=length/param, xx=nothing

    # XXX use rlwinm here
    srwi    r8,  r3,  16 # save cmd+param for responses
    srwi    r4,  r3,  24 # get command
    cmpwi   r4,  NUM_CMDS
    bge     .onExiInterrupt_end # invalid, just ignore it...
    slwi    r4,  r4,  1  # to offset
    srwi    r9,  r3,  16 # get param
    andi.   r9,  r3,  0xFF
    addi    r5,  r14, exiCmdTbl - onExiInterrupt
    lhax    r5,  r4,  r5
    mtspr   CTR, r5
    bctr    # call cmd handler


.onExiInterrupt_end:
    stw     r30, (EXI_IO_BASE+EXI1CSR)(r15) # restore previous CSR

.onExiInterrupt_skip:
    lwz     r9, SP_LR_SAVE(r1)
    mtlr    r9 # restore LR
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    JUMP    0x80243c54, r0


_exiWait: # start EXI op and wait for it to finish. r3 = CR
    # clobbers r3
    stw     r3,  (EXI_IO_BASE+EXI1CR)(r15)
    ._exiWait_loop:
    lwz     r3,  (EXI_IO_BASE+EXI1CR)(r15)
    andi.   r3,  r3,  1
    bne     ._exiWait_loop
    blr

exiRead16: # read 16-bit value from EXI. return in r3
    # clobbers r3, r12
    mflr    r12
    li      r3,  EXI_CR_LEN_2 | EXI_CR_READ | EXI_CR_IMM | EXI_CR_START
    bl      _exiWait
    lhz     r3,  (EXI_IO_BASE+EXI1DATA)(r15)
bl_r12:
    mtlr    r12
    blr

exiRead32: # read 32-bit value
    # clobbers r4, r5, r12; returns in r3
    # since our adapter only does 16-bit I/O we need to do two reads
    mflr    r12
    li      r4,  EXI_CR_LEN_2 | EXI_CR_READ | EXI_CR_IMM | EXI_CR_START
    mr      r3,  r4
    bl      _exiWait
    lhz     r5,  (EXI_IO_BASE+EXI1DATA)(r15) # high word
    slwi    r5,  r5,  16 # save for later
    mr      r3,  r4
    bl      _exiWait
    lhz     r3,  (EXI_IO_BASE+EXI1DATA)(r15) # low word
    or      r3,  r3,  r5
    b       bl_r12

exiSend16: # send 16-bit value in r3
    # clobbers r3
    slwi    r3,  r3,  16 # XXX necessary? can we just use sth?
    stw     r3,  (EXI_IO_BASE+EXI1DATA)(r15)
    li      r3,  EXI_CR_LEN_2 | EXI_CR_WRITE | EXI_CR_IMM | EXI_CR_START
    b       _exiWait

exiSend32: # send 32-bit value in r3
    # clobbers r3, r4, r12
    mflr    r12
    mr      r4,  r3
    slwi    r3,  r3,  16 # XXX necessary? can we just use sth?
    stw     r3,  (EXI_IO_BASE+EXI1DATA)(r15)
    li      r3,  EXI_CR_LEN_2 | EXI_CR_WRITE | EXI_CR_IMM | EXI_CR_START
    bl      _exiWait
    slwi    r3,  r4,  16 # XXX necessary? can we just use sth?
    stw     r3,  (EXI_IO_BASE+EXI1DATA)(r15)
    li      r3,  EXI_CR_LEN_2 | EXI_CR_WRITE | EXI_CR_IMM | EXI_CR_START
    mtlr    r12
    b       _exiWait


exiCmdTbl:
    .short exiCmdMisc       - onExiInterrupt # CMD_MISC
    .short exiCmdReadMem    - onExiInterrupt # CMD_READ_MEM
    .short exiCmdWriteMem   - onExiInterrupt # CMD_WRITE_MEM
    .short exiCmdReadRegs   - onExiInterrupt # CMD_READ_REGS
    .short exiCmdWriteRegs  - onExiInterrupt # CMD_WRITE_REGS
    .short exiCmdFlushCache - onExiInterrupt # CMD_FLUSH_CACHE
    .short exiCmdCopyMem    - onExiInterrupt # CMD_COPY_MEM
    .short exiCmdFillMem    - onExiInterrupt # CMD_FILL_MEM
    .short exiCmdSearchMem  - onExiInterrupt # CMD_SEARCH_MEM
    .short exiCmdCallFunc   - onExiInterrupt # CMD_CALL_FUNC
    .short exiCmdExecCode   - onExiInterrupt # CMD_EXEC_CODE
    .short exiCmdBreakpoint - onExiInterrupt # CMD_BREAKPOINT

exiCmdMisc: # CMD_MISC; r9 subcmd
    # TODO
    b       .onExiInterrupt_end


exiCmdReadMem: # CMD_READ_MEM
    # r8: cmd and param (01cc)
    # r9: count (cc)
    cmpwi   r9,  0
    bne     .readMemGetAddr
    bl      exiRead32 # get count
    mr      r9,  r3
.readMemGetAddr:
    bl      exiRead32 # r3 = address
    subi    r4,  r3,  2 # r4 = address - 2 (save + offset for lhzu)
    mr      r3,  r8 # cmd+param (response header)
.readMemNext:
    bl      exiSend16
    lhzu    r3,  2(r4)
    subi    r9,  r9,  1
    cmpwi   r9,  0
    bne     .readMemNext
    b       .onExiInterrupt_end


exiCmdWriteMem: # CMD_WRITE_MEM
    # r8: cmd and param (01cc)
    # r9: count (cc)
    cmpwi   r9,  0
    bne     .writeMemGetAddr
    bl      exiRead32 # get count
    mr      r9,  r3
.writeMemGetAddr:
    bl      exiRead32 # r3 = address
    subi    r4,  r3,  2 # r4 = address - 2 (save + offset for lhzu)
    li      r3,  CMD_WRITE_MEM # response header: 00nn
    bl      exiSend16
.writeMemNext:
    bl      exiRead16
    sthu    r3,  2(r4)
    subi    r9,  r9,  1
    cmpwi   r9,  0
    bne     .writeMemNext
    b       .onExiInterrupt_end


exiCmdReadRegs:
    # XXX would need to check context of interrupted thread
    b       .onExiInterrupt_end
exiCmdWriteRegs:
    b       .onExiInterrupt_end


exiCmdFlushCache: # r9=count, next=addr
    li      r3,  CMD_FLUSH_CACHE # response header: 00nn
    bl      exiSend16
    bl      exiRead32 # get addr
    mr      r4,  r9
    CALL    0x80003374
    b       .onExiInterrupt_end


exiCmdCopyMem: #r9=count (bytes), next=u32 src, u32 dest
    cmpwi   r9,  0
    bne     .copyMemGetAddr
    bl      exiRead32
    mr      r9,  r3
.copyMemGetAddr:
    bl      exiRead32 # r3 = src
    mr      r7,  r3
    bl      exiRead32 # r3 = dest
    mr      r4,  r7 # restore src
    mr      r5,  r9 # count
    CALL    memcpy # r3=dest r4=src r5=len
    b       .onExiInterrupt_end

exiCmdFillMem:
    cmpwi   r9,  0
    bne     .fillMemGetAddr
    bl      exiRead32
    mr      r9,  r3
.fillMemGetAddr:
    bl      exiRead32 # r3 = dest
    mr      r7,  r3
    bl      exiRead16 # r3 = val
    mr      r4,  r3
    slwi    r3,  r3,  16
    or      r4,  r4,  r3 # both halves of r4
    # XXX the game's memset only uses the low byte anyway...
    mr      r3,  r7 # restore dest
    mr      r5,  r9 # count
    CALL    memset # dest, val, len
    b       .onExiInterrupt_end


exiCmdSearchMem:
    b       .onExiInterrupt_end

exiCmdCallFunc:
    # XXX response header
    bl      exiRead32 # get ffrrxx00
    mr      r17, r3
    bl      exiRead32 # get address
    mr      r18, r3
    addi    r16, r14, (buffer - .onExiInterrupt_getpc) - 4
    addi    r19, r14, .callFunc_do - .onExiInterrupt_getpc
    # r9 = number of params
.callFunc_nextParam:
    cmpwi   r9,  0
    beq     .callFunc_doneParams
    bl      exiRead32 # get next param
    stwu    r3,  4(r16) # store to buffer
    subi    r19, r19, 4 # previous label (XXX range check)
    subi    r9,  r9,  1 # dec param count
    b       .callFunc_nextParam
.callFunc_doneParams:
    mtspr   CTR, r19
    bctr    # jump to one of the labels below.
    # XXX floats

.callFunc_r12: lwzu r12, -4(r16)
.callFunc_r11: lwzu r11, -4(r16)
.callFunc_r10: lwzu r10, -4(r16)
.callFunc_r9:  lwzu r9,  -4(r16)
.callFunc_r8:  lwzu r8,  -4(r16)
.callFunc_r7:  lwzu r7,  -4(r16)
.callFunc_r6:  lwzu r6,  -4(r16)
.callFunc_r5:  lwzu r5,  -4(r16)
.callFunc_r4:  lwzu r4,  -4(r16)
.callFunc_r3:  lwzu r3,  -4(r16)
.callFunc_do:
    mtspr   CTR, r18
    bctrl   # call it!
    # store the results
    addi    r16, r14, buffer - .onExiInterrupt_getpc # XXX necessary?
    stmw    r3,  0(r16) # uses 116 bytes of buffer
    subi    r16, r16, 4 # for lwzu

    # r17 is ffrrxx00; ff=n float params, rr=n return params, xx=n return floats
    srwi    r18, r17, 16
    andi.   r18, r18, 0xFF # XXX rlwinm
.callFunc_nextReturn:
    cmpwi   r18, 0
    beq     .onExiInterrupt_end
    subi    r18, r18, 1
    lwzu    r3,  4(r16)
    bl      exiSend32
    b       .callFunc_nextReturn
    # XXX floats


exiCmdExecCode:
    b       .onExiInterrupt_end

exiCmdBreakpoint:
    b       .onExiInterrupt_end


buffer: .rept 128 # minimum 116
.byte 0
.endr
