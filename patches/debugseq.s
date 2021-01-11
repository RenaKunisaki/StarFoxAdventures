# Sequence debug patch:
# logs some sequence operations.
.text
.include "common.s"
.set LOG_CURVE_ACTIONS,1
.set LOG_SUBCMD_0B,1
.set LOG_OBJ_MSGS,1

# define patches
patchList:
    PATCH_ID  "seqDbug" # must be 7 chars
.if LOG_CURVE_ACTIONS
    PATCH_B   0x800853c0, onCurveAction
.endif
.if LOG_SUBCMD_0B
    PATCH_B   0x80083784, onSubCmd0B
.endif
.if LOG_OBJ_MSGS
    PATCH_B   0x800375b4, onMsgSendNear
    PATCH_B   0x800376ec, onMsgSend
    PATCH_B   0x800378d0, onMsgSendTo
.endif
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE, 0x80 # how much to reserve
    .set SP_LR_SAVE, 0x84
    .set SP_GPR_SAVE,0x10

entry: # called as soon as our patch is loaded.
    blr # nothing to do here


# 800853c0 88 9A 00 00  lbz       model,0x0(cmd.cmd)
.if LOG_CURVE_ACTIONS
onCurveAction:
    lbz     r4,  0x00(r26) # replaced
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3,  SP_LR_SAVE(r1)

    bl .onCurveAction_getpc
    .onCurveAction_getpc: mflr r14

    lbz     r5,  0x00(r26) # cmd
    lbz     r6,  0x01(r26) # time
    lhz     r7,  0x02(r26) # param
    LOADW   r4,  frameCount
    addi    r3,  r14, (.s_seqCmd - .onCurveAction_getpc)@l
    CALL    OSReport

    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    JUMP    0x800853c4, r0 # return to original code
.endif # LOG_CURVE_ACTIONS


.if LOG_SUBCMD_0B
onSubCmd0B:
    # r3:  cond
    # r22: bSkip
    # r23: bDontExecute
    # r25: iCmd
    # r26: idx
    # r27: param
    # r28: length
    # r29: opcode
    # r31: cmd*
    li      r3,  0 # replaced
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3,  SP_LR_SAVE(r1)

    bl .onSubCmd0B_getpc
    .onSubCmd0B_getpc: mflr r14

    LOADW   r4,  frameCount
    mr      r5,  r29 # op
    mr      r6,  r26 # idx
    mr      r7,  r27 # param
    mr      r8,  r23 # bDontExecute
    mr      r9,  r22 # bSkip
    addi    r3,  r14, (.s_cmd0B - .onSubCmd0B_getpc)@l
    CALL    OSReport

    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    JUMP    0x80083788, r0 # return to original code
.endif # LOG_SUBCMD_0B


.if LOG_OBJ_MSGS
onMsgSendNear:
    # r3:  defNo
    # r4:  flags
    # r5:  from
    # r6:  msg
    # r7:  param
    # f1:  maxDistance
    or      r25, r3,  r3 # replaced
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3,  SP_LR_SAVE(r1)

    bl .onMsgSendNear_getpc
    .onMsgSendNear_getpc: mflr r14

    mr      r0,  r5  # save 'from'
    mr      r5,  r6  # msg
    mr      r6,  r7  # param
    mr      r7,  r0  # from
    lwz     r8,  0x50(r7)
    addi    r8,  r8,  0x91 # name
    mr      r9,  r25 # defNo
    mr      r10, r4  # flags
    LOADW   r4,  frameCount
    addi    r3,  r14, (.s_objMsgNear - .onMsgSendNear_getpc)@l
    MAGIC_FLOAT_INCANTATION
    CALL    OSReport

    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    JUMP    0x800375b8, r0 # return to original code


onMsgSend:
    # r3:  objId (defNo or catId)
    # r4:  flags
    # r5:  from
    # r6:  msg
    # r7:  param
    or      r25, r3,  r3 # replaced
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3,  SP_LR_SAVE(r1)

    bl .onMsgSend_getpc
    .onMsgSend_getpc: mflr r14

    mr      r11, r4  # flags
    mr      r0,  r5  # save 'from'
    mr      r5,  r6  # msg
    mr      r6,  r7  # param
    mr      r7,  r0  # from
    lwz     r8,  0x50(r7)
    addi    r8,  r8,  0x91 # name
    addi    r9,  r14, (.s_catId - .onMsgSend_getpc)@l
    andi.   r0,  r4,  0x04 # flags & UseDefNo
    beq     .onMsgSend_useCatId
    addi    r9,  r14, (.s_defNo - .onMsgSend_getpc)@l
.onMsgSend_useCatId:
    mr      r10, r3  # defNo or catId
    LOADW   r4,  frameCount
    addi    r3,  r14, (.s_objMsg - .onMsgSend_getpc)@l
    CALL    OSReport

    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    JUMP    0x800376f0, r0 # return to original code


onMsgSendTo:
    # r3:  targret
    # r4:  msg
    # r5:  from
    # r6:  param
    or      r8,  r3,  r3 # replaced
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3,  SP_LR_SAVE(r1)

    bl .onMsgSendTo_getpc
    .onMsgSendTo_getpc: mflr r14

    mr      r7,  r5  # from
    mr      r9,  r8  # to
    mr      r5,  r4  # msg
    # r6 is already param
    lwz     r8,  0x50(r7)
    addi    r8,  r8,  0x91 # from name
    lwz     r10, 0x50(r9)
    addi    r10, r10, 0x91 # to name
    LOADW   r4,  frameCount
    addi    r3,  r14, (.s_objMsgTo - .onMsgSendTo_getpc)@l
    CALL    OSReport

    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    JUMP    0x800378d4, r0 # return to original code
.endif # LOG_OBJ_MSGS

.if LOG_CURVE_ACTIONS #   r4           r5     r6         r7
    .s_seqCmd: .string "[F%9d] SEQCMD %02X T %02X param %04X"
.endif
.if LOG_SUBCMD_0B #      r4              r5       r6         r7       r8 r9
    .s_cmd0B: .string "[F%9d] SEQCMD 0B:%02X idx %02X param %04X skip=%d,%d"
.endif
.if LOG_OBJ_MSGS #            r4           r5         r6        r7  r8            r9       r10      f1
    .s_objMsgNear: .string "[F%9d] OBJMSG %08X param %08X from %08X[%s] to defNo %04X flg %04X dist %f"
    #                    r4            r5         r6        r7  r8     r9  r10     r11
    .s_objMsg: .string "[F%9d] OBJMSG %08X param %08X from %08X[%s] to %s %08X flg %04X"
    #                       r4           r5         r6        r7  r8      r9  r10
    .s_objMsgTo: .string "[F%9d] OBJMSG %08X param %08X from %08X[%s] to %08X[%s]"
    .s_defNo: .string "defNo"
    .s_catId: .string "catId"
.endif
