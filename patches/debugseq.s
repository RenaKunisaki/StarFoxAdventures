# Sequence debug patch:
# logs some sequence operations.
.text
.include "common.s"
.set LOG_SEQ_START,1
.set LOG_CURVE_ACTIONS,1
.set LOG_SUBCMD_0B,1
.set LOG_OBJ_MSGS,1
.set LOG_ROT_BUG,1

# define patches
patchList:
    PATCH_ID  "seqDbug" # must be 7 chars
.if LOG_SEQ_START
    PATCH_B   0x80080e14, onSeqStart
    PATCH_B   0x80080be4, onSeqPreempt
    PATCH_B   0x80080bcc, onSeqYield
    PATCH_B   0x800822d4, onLoadAnimData
    PATCH_B   0x8016c3bc, onSetupAnimObj
.endif
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
.if LOG_ROT_BUG
    PATCH_B   0x80080ac8, onRotBug
.endif
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE, 0x80 # how much to reserve
    .set SP_LR_SAVE, 0x84
    .set SP_GPR_SAVE,0x10

entry: # called as soon as our patch is loaded.
    blr # nothing to do here


.if LOG_SEQ_START
onSeqStart:
    # r3:  seqIdx
    # r4:  obj
    # r5:  objs
    or      r20, r3,  r3 # replaced
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3,  SP_LR_SAVE(r1)

    bl .onSeqStart_getpc
    .onSeqStart_getpc: mflr r14

    mr      r7,  r4  # obj
    mr      r6,  r5  # objs
    mr      r5,  r20 # seq
    addi    r8,  r14, (.s_noObj - .onSeqStart_getpc)
    andis.  r0,  r7,  0x8000
    beq     .onSeqStart_noObj
    lwz     r8,  0x50(r7) # ObjectFileStruct*
    andis.  r0,  r8,  0x8000
    beq     .onSeqStart_noObj
    addi    r8,  r8,  0x91 # name
.onSeqStart_noObj:
    LOADW   r4,  frameCount
    addi    r3,  r14, (.s_seqStart - .onSeqStart_getpc)@l
    CALL    OSReport

    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    JUMP    0x80080e18, r0 # return to original code


onSeqPreempt:
    # r3: obj, r4: ?
    lbz     r6,  -0x60bc(r13) # replaced
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3,  SP_LR_SAVE(r1)

    bl .onSeqPreempt_getpc
    .onSeqPreempt_getpc: mflr r14

    mr      r5,  r3  # obj
    addi    r6,  r14, (.s_noObj - .onSeqPreempt_getpc)
    mr      r7,  r4  # param
    andis.  r0,  r5,  0x8000
    beq     .onSeqPreempt_noObj
    lwz     r6,  0x50(r5) # ObjectFileStruct
    andis.  r0,  r6,  0x8000
    beq     .onSeqPreempt_noObj
    addi    r6,  r6,  0x91 # name
.onSeqPreempt_noObj:
    LOADW   r4,  frameCount
    addi    r3,  r14, (.s_preempt - .onSeqPreempt_getpc)@l
    CALL    OSReport

    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    JUMP    0x80080be8, r0 # return to original code


# 80080bcc 90 83 00 74  stw       time,state->nextTime(r3)
#                     r4                  r5  r6     r7
#.s_yield: .string "[F%9d] SEQ YIELD Obj %08X(%s) t=%04X"
onSeqYield:
    # r3: state, r4: time
    stw     r4,  0x74(r3) # replaced
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)

    bl .onSeqYield_getpc
    .onSeqYield_getpc: mflr r14

    mr      r7,  r4  # time
    lwz     r5,  0x00(r3) # obj
    addi    r6,  r14, (.s_noObj - .onSeqYield_getpc)
    andis.  r0,  r5,  0x8000
    beq     .onSeqYield_noObj
    lwz     r6,  0x50(r5) # ObjectFileStruct
    andis.  r0,  r6,  0x8000
    beq     .onSeqYield_noObj
    addi    r6,  r6,  0x91 # name
.onSeqYield_noObj:
    LOADW   r4,  frameCount
    addi    r3,  r14, (.s_yield - .onSeqYield_getpc)@l
    CALL    OSReport

    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    JUMP    0x80080bd0, r0 # return to original code

onLoadAnimData: # 800822d4 38 60 00 0E  li r3,0xe
    # r28: realIdx
    # r29: ObjState_AnimatedObj*
    # r30: objDef*
    li      r3,  0x0E # replaced
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)

    bl .onLoadAnimData_getpc
    .onLoadAnimData_getpc: mflr r14

    lhz     r5,  0x00(r30) # objDefNo
    lwz     r6,  0x14(r30) # unique ID
    lhz     r7,  0x18(r30) # seqNo
    mr      r8,  r28       # seqIdx
    LOADW   r4,  frameCount
    addi    r3,  r14, (.s_loadAnim - .onLoadAnimData_getpc)@l
    CALL    OSReport

    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    JUMP    0x800822d8, r0 # return to original code

onSetupAnimObj: # 0x8016c3bc 80 9E 00 F4  lwz r4,0xf4(r30)
    # r30: ObjInstance*
    # r31: ObjDef*
    lwz     r4,  0xf4(r30) # replaced
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)

    bl .onSetupAnimObj_getpc
    .onSetupAnimObj_getpc: mflr r14

    lhz     r5,  0x00(r31) # objDefNo
    mr      r6,  r30 # ObjInstance*
    lwz     r7,  0x50(r30)
    addi    r7,  r7,  0x91 # name
    lhz     r8,  0x18(r31) # seqNo
    LOADW   r4,  frameCount
    addi    r3,  r14, (.s_setupAnimObj - .onSetupAnimObj_getpc)@l
    CALL    OSReport

    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    JUMP    0x8016c3c0, r0 # return to original code


.endif # LOG_SEQ_START


.if LOG_CURVE_ACTIONS
onCurveAction:
    # r25: obj
    # r26: cmd
    # r28: state
    lbz     r4,  0x00(r26) # replaced
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3,  SP_LR_SAVE(r1)

    bl .onCurveAction_getpc
    .onCurveAction_getpc: mflr r14

    # 0x66 iAction, 0x62 nActions, 0x57 seqIdx
    mr      r5,  r25       # obj
    addi    r6,  r14, (.s_noObj - .onCurveAction_getpc)
    andis.  r0,  r5,  0x8000
    beq     .onCurveAction_noObj
    lwz     r6,  0x50(r5)
    andis.  r0,  r6,  0x8000
    beq     .onCurveAction_noObj
    addi    r6,  r6,  0x91 # name
.onCurveAction_noObj:
    lhz     r7,  0x66(r28) # iAction
    lhz     r8,  0x62(r28) # nActions
    lbz     r9,  0x00(r26) # cmd
    lhz     r10, 0x02(r26) # param
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
    addi    r8,  r14, (.s_noObj - .onMsgSendNear_getpc)
    andis.  r0,  r7,  0x8000
    beq     .onMsgSendNear_noFrom
    lwz     r8,  0x50(r7)
    andis.  r0,  r8,  0x8000
    beq     .onMsgSendNear_noFrom
    addi    r8,  r8,  0x91 # name
.onMsgSendNear_noFrom:
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
    addi    r8,  r14, (.s_noObj - .onMsgSend_getpc)
    andis.  r0,  r7,  0x8000
    beq     .onMsgSend_noFrom
    lwz     r8,  0x50(r7)
    andis.  r0,  r8,  0x8000
    beq     .onMsgSend_noFrom
    addi    r8,  r8,  0x91 # name
.onMsgSend_noFrom:
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
    addi    r8,  r14, (.s_noObj - .onMsgSendTo_getpc)
    addi    r10, r14, (.s_noObj - .onMsgSendTo_getpc)
    andis.  r0,  r7,  0x8000
    beq     .onMsgSendTo_noFrom
    lwz     r8,  0x50(r7)
    andis.  r0,  r8,  0x8000
    beq     .onMsgSendTo_noFrom
    addi    r8,  r8,  0x91 # from name
.onMsgSendTo_noFrom:
    andis.  r0,  r9,  0x8000
    beq     .onMsgSendTo_noTo
    lwz     r10, 0x50(r9)
    andis.  r0,  r10, 0x8000
    beq     .onMsgSendTo_noTo
    addi    r10, r10, 0x91 # to name
.onMsgSendTo_noTo:
    LOADW   r4,  frameCount
    addi    r3,  r14, (.s_objMsgTo - .onMsgSendTo_getpc)@l
    CALL    OSReport

    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    JUMP    0x800378d4, r0 # return to original code
.endif # LOG_OBJ_MSGS


#80080ac8 38 81 00 08  addi      state,r1,offset DAT_803f8400      = FFF80000h
#.if LOG_ROT_BUG #         r4                               r5  r6       r7   r8
#    .s_rotBug: .string "[F%9D] Using bugged constant! Obj %08X(%s) cmd %02X/%02X"
.if LOG_ROT_BUG
onRotBug:
    addi    r4,  r1,  8 # replaced
    # r3:  obj
    # r4:  state
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)

    bl .onRotBug_getpc
    .onRotBug_getpc: mflr r14

    mr      r5,  r3  # obj
    addi    r6,  r14, (.s_noObj - .onRotBug_getpc)
    andis.  r0,  r5,  0x8000
    beq     .onRotBug_noObj
    lwz     r6,  0x50(r5)
    andis.  r0,  r6,  0x8000
    beq     .onRotBug_noObj
    addi    r6,  r6,  0x91 # name
.onRotBug_noObj:
    lhz     r7,  0x66(r4) # iAction
    lhz     r8,  0x62(r4) # nActions
    LOADW   r4,  frameCount
    addi    r3,  r14, (.s_rotBug - .onRotBug_getpc)@l
    CALL    OSReport

    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    JUMP    0x80080acc, r0 # return to original code
.endif # LOG_ROT_BUG



.if LOG_SEQ_START #         r4              r5   r6       r7  r8
    .s_seqStart: .string "[F%9d] SEQ START %04X %08X Obj %08X %s"
    #                      r4                    r5  r6   r7
    .s_preempt: .string "[F%9d] SEQ PREEMPT Obj %08X(%s) %04X"
    #                    r4                  r5  r6     r7
    .s_yield: .string "[F%9d] SEQ YIELD Obj %08X(%s) t=%04X"
    #                       r4                    r5      r6       r7    r8
    .s_loadAnim: .string "[F%9d] LOADANIM ObjDef %04X ID %08X seq %04X (%04X)"
    #                           r4                  r5   r6  r7       r8
    .s_setupAnimObj: .string "[F%9d] SETUP ANIMOBJ %04X %08X[%s] seq %04X"
.endif
.if LOG_CURVE_ACTIONS #   r4           r5    r6   r7          r8       r9  r10
    #.s_seqCmd: .string "[F%9d] SEQCMD %02X [%02X/%02X] param %04X obj %08X(%s)"
    #                     r4               r5  r6    r7   r8        r9         r10
    .s_seqCmd: .string "[F%9d] SEQCMD Obj %08X(%s) #%02X/%02X: Cmd %02X param %04X"
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
    .s_noObj: .string "-"
.endif
.if LOG_ROT_BUG #         r4                               r5  r6       r7   r8
    .s_rotBug: .string "[F%9D] Using bugged constant! Obj %08X(%s) cmd %02X/%02X"
.endif
