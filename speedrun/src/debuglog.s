# Restore debugPrintf function.
.text
.include "common.s"
.global debugPrintfHook
debugPrintfHook:
    # copied code
    stwu    r1, -0x80(r1) # replaced (also increased to 0x80)

    bne     cr1, LAB_80137970
    stfd    f1,  0x28(r1)
    stfd    f2,  0x30(r1)
    stfd    f3,  0x38(r1)
    stfd    f4,  0x40(r1)
    stfd    f5,  0x48(r1)
    stfd    f6,  0x50(r1)
    stfd    f7,  0x58(r1)
    stfd    f8,  0x60(r1)
LAB_80137970:
    stw     r3,  0x08(r1)
    stw     r4,  0x0C(r1)
    stw     r5,  0x10(r1)
    stw     r6,  0x14(r1)
    stw     r7,  0x18(r1)
    stw     r8,  0x1C(r1)
    stw     r9,  0x20(r1)
    stw     r10, 0x24(r1)

    # new code
    mflr    r11
    stw     r11, 0x68(r1) # stash lr too
    stw     r14, 0x6C(r1)

    bl .debugPrintfHook_getpc
    .debugPrintfHook_getpc:
        mflr r14

    # don't draw if debug text is off
    lwz     r3,  (.offsEnableDebugText - .debugPrintfHook_getpc)(r14)
    lbz     r3,  0(r3)
    cmpwi   r3,  0
    beq     .debugTextDisabled

    # prepare params for sprintf
    LOADW   r3,  debugLogEnd
    lwz     r4,  0x08(r1) # must shift all params up one register
    lwz     r5,  0x0C(r1)
    lwz     r6,  0x10(r1)
    lwz     r7,  0x14(r1)
    lwz     r8,  0x18(r1)
    lwz     r9,  0x1C(r1)
    lwz     r10, 0x20(r1)
    lwz     r11, 0x24(r1)
    CALL    sprintf
    # XXX this is probably why it breaks when given too many params.
    # we need to account for stack params too.

    # adjust debugLogEnd
    LOADWH  r4,  debugLogEnd
    LOADWL2 r5,  debugLogEnd, r4
    add     r5,  r5,  r3
    addi    r5,  r5,  1   # or else it deadlocks displaying
    STOREW  r5,  debugLogEnd, r4

.debugTextDisabled:
    # restore regs
    lwz     r5,  0x68(r1)
    mtlr    r5
    lwz     r14, 0x6C(r1)

    # copied code
    addi    r1,  r1,  0x80
    blr

.offsEnableDebugText:
    .int enableDebugText
