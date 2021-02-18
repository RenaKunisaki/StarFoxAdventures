# pad3 patch:
# adds some additional functions to controller 3:
# - hold Z: fast forward
# - press X: toggle free move
# also, enables some debug functions left in the original game:
# - press A: print memory info to console (rather useless since it does this
#   automatically every few seconds anyway)
# - press B: refresh textures
.text
.include "common.s"
.include "globals.s"

# define patches
patchList:
    PATCH_ID        "Pad3   " # must be 7 chars
    PATCH_MAIN_LOOP mainLoop

    # enable all four controllers,
    # which enables at least one debug function
    PATCH_BYTE 0x80014B87, 0x00000004
    PATCH_BYTE 0x80014BC7, 0x00000004
    PATCH_BYTE 0x80014C1B, 0x00000004
    PATCH_BYTE 0x80014C6F, 0x00000004
    PATCH_BYTE 0x80014CC3, 0x00000004
    PATCH_BYTE 0x80014D9F, 0x00000004
    PATCH_BYTE 0x80014DDB, 0x00000004
    PATCH_BYTE 0x80014E17, 0x00000004
    PATCH_BYTE 0x80014E73, 0x00000004
    PATCH_BYTE 0x80014EC7, 0x00000004
    PATCH_BYTE 0x80014EEB, 0x00000004

    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,  0x80 # how much to reserve
    .set SP_LR_SAVE,  0x84
    .set SP_GPR_SAVE, 0x10

entry: # called as soon as our patch is loaded.
    blr

mainLoop: # called from main loop. r3 = mainLoop
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r5
    stw     r5, SP_LR_SAVE(r1)
    stmw    r3, SP_GPR_SAVE(r1)
    mr      r14, r3

    LOADWH  r15, controllerStates
    LOADHL2 r3,  controller3state, r15 # get buttons

    # get current time scale
    LOADWH  r16, physicsTimeScale
    LOADWL2 r4,  physicsTimeScale, r16
    li      r6,  0
    lbz     r5,  (wasZ - mainLoop)(r14)
    cmpwi   r5,  0
    bne     .wasZBefore
    # if we weren't holding Z before, save that
    # as the previous speed.
    stw     r4,  (oldSpeed - mainLoop)(r14)

    # check for Z: fast forward while held
.wasZBefore:
    lwz     r4,  (oldSpeed - mainLoop)(r14)
    andi.   r0,  r3,  PAD_BUTTON_Z
    beq     .notZ
    lis     r4,  0x4334 # 180.0, ie 300% speed
    li      r6,  1
.notZ:
    STOREW  r4,  physicsTimeScale, r16
    stb     r6,  (wasZ - mainLoop)(r14)

    # check for X: toggle free move
    LOADHL2 r3,  0x80339908, r15
    li      r4,  0
    andi.   r0,  r3,  PAD_BUTTON_X
    beq     .notX

    lbz     r6,  (wasX - mainLoop)(r14)
    cmpwi   r6,  0
    bne     .end

    LOADW   r3,  PATCH_STATE_PTR
    lbz     r4,  ENABLE_FREE_MOVE(r3)
    xori    r4,  r4,  1
    stb     r4,  ENABLE_FREE_MOVE(r3)
    li      r4,  1

.notX:
    stb     r4,  (wasX - mainLoop)(r14)
.end:
    lwz     r5,  SP_LR_SAVE(r1)
    mtlr    r5   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1, STACK_SIZE # restore stack ptr
    blr


oldSpeed: .float 60 # previous game speed
wasZ: .byte 0 # Z held before?
wasX: .byte 0
.align 4
