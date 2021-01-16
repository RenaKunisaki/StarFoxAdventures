.ascii "dbgmisc " # 8 byte file ID for debug

drawItem_crash:
    addi    r4,  r14, (s_CrashGame - mainLoop)
    blr

adjItem_crash: # r3 = amount to adjust by (0=A button)
    cmpwi   r3,  0
    bnelr
    li      r3,  0xFFFFDEAD
    li      r4,  0
    stw     r3,  0xBAD(r4)

#######################################################################

drawItem_reboot:
    addi    r4,  r14, (s_Reboot - mainLoop)
    blr

adjItem_reboot: # r3 = amount to adjust by (0=A button)
    cmpwi   r3,  0
    bnelr
    LOADWH  r3,  IO_BASE
    stw     r3,  (IRQ_IO_BASE+CPU_RESET)(r3) # store anything to reset
    blr

#######################################################################

drawItem_exiTest:
    addi    r4,  r14, (s_ExiTest - mainLoop)
    lwz     r5,  (exiTestResult - mainLoop)(r14)
    blr

adjItem_exiTest: # r3 = amount to adjust by (0=A button)
    cmpwi   r3,  0
    bnelr
    LOADWH  r3,  IO_BASE
    li      r4,  0xD0 # 32MHz, device 2
    stw     r4,  (EXI_IO_BASE+EXI1CSR)(r3)
    LOAD    r4,  0x89ABCDEF
    stw     r4,  (EXI_IO_BASE+EXI1DATA)(r3)
    li      r4,  0x19 # start, r/w mode, 2 bytes
    stw     r4,  (EXI_IO_BASE+EXI1CR)(r3)
.adjItem_exiTest_wait:
    lwz     r4,  (EXI_IO_BASE+EXI1CR)(r3)
    andi.   r4,  r4,  1
    bne     .adjItem_exiTest_wait
    lwz     r4,  (EXI_IO_BASE+EXI1DATA)(r3)
    stw     r4,  (exiTestResult - mainLoop)(r14)
    blr

#######################################################################
