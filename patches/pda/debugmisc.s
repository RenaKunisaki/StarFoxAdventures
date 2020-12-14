.ascii "dbgmisc " # 8 byte file ID for debug

drawItem_crash:
    addi    r4,  r14, (s_CrashGame - mainLoop)
    blr

adjItem_crash: # r3 = amount to adjust by (0=A button)
    li     r3,  0xFFFFDEAD
    li     r4,  0
    stw    r3,  0xBAD(r4)

#######################################################################
