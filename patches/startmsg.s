# start message patch:
# displays message at startup.
.text
.include "common.s"
.include "globals.s"

# define patches
patchList:
    PATCH_ID   "strtmsg" # must be 7 chars
    #PATCH_WORD 0x80115D04, 0x28000001 # make message show up on frame 1
    #PATCH_WORD 0x80115D14, 0x60000000 # make it show up during disc access
    #PATCH_WORD 0x80115CFC, 0x60000000 # make it show up while loading
    PATCH_B    0x80115cf4, main # hook into loading screen
    PATCH_STARTUP doStartup
    PATCH_END  PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE, 0xC0 # how much to reserve
    .set SP_LR_SAVE, 0xC4
    .set SP_GPR_SAVE,0x10

entry: # called as soon as our patch is loaded.
    blr

doStartup: # called from late init
    # we can't just check initFrameCount because if the game loads
    # very quickly (eg in Dolphin with fast disc access on),
    # it might not get very high.
    stwu     r1, -STACK_SIZE(r1) # get some stack space
    mflr     r9
    stw      r9, SP_LR_SAVE(r1)
    stmw     r3, SP_GPR_SAVE(r1)

    # if Z isn't held, turn debug text back off.
    li       r3,  0
    LOADWH   r9,  controllerStates
    LOADHL2  r6,  controllerStates, r9 # buttons
    andi.    r0,  r6, PAD_BUTTON_Z
    beq      .notZheld
    li       r3,  1
.notZheld:
    LOADWH   r4,  enableDebugText
    STOREB   r3,  enableDebugText, r4

    andi.    r0,  r6, PAD_BUTTON_Y | PAD_BUTTON_B
    cmpwi    r0,      PAD_BUTTON_Y | PAD_BUTTON_B
    bne      .doStartup_end

    LOADW    r6, PATCH_STATE_PTR
    li       r5, 0x80
    stb      r5, CUR_CHAR_ADDR(r6)
.doStartup_end:
    lwz      r0, SP_LR_SAVE(r1)
    mtlr     r0 # restore LR
    lmw      r3, SP_GPR_SAVE(r1)
    addi     r1, r1, STACK_SIZE # restore stack ptr
    blr

main: # called from loading screen loop
    stwu     r1, -STACK_SIZE(r1) # get some stack space
    mflr     r9
    stw      r9, SP_LR_SAVE(r1)
    stmw     r3, SP_GPR_SAVE(r1)

    bl .getpc
    .getpc:  mflr r14
    LOADWH   r4,  enableDebugText

    # get initFrameCount
    lwz      r0,  -0x5bf4(r13)

    # gametext is a butt and it would be difficult to make it
    # actually set up how we want, and we don't have enough
    # free RAM at this point to load a texture, so we'll
    # have to use debug text.
    li       r3,  1
    STOREB   r3,  enableDebugText, r4

    li       r3,  60 # X
    li       r4, 420 # Y
    addi     r5, r14, (.msg3 - .getpc)@l

    cmpwi    r0, 400
    bge      .doPrint
    addi     r5, r14, (.msg2 - .getpc)@l

    cmpwi    r0, 200
    bge      .doPrint
    addi     r5, r14, (.msg1 - .getpc)@l

.doPrint:
    CALL     debugPrintfxy

.end:
    lwz      r0, SP_LR_SAVE(r1)
    mtlr     r0 # restore LR
    lmw      r3, SP_GPR_SAVE(r1)
    addi     r1, r1, STACK_SIZE # restore stack ptr
    JUMP     0x80115d44, r0 # jump back to game code


.msg1: .string "Amethyst Edition v1.0"
.msg2: .string "segment6.net"
.msg3: .ascii "This is a free fan creation.\n"
    .ascii "If you paid for this, you've been scammed.\0"
