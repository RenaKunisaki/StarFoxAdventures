# DLL debug patch:
# logs DLL load/free operations.
.text
.include "common.s"

# define patches
patchList:
    PATCH_ID  "dllLog " # must be 7 chars
    PATCH_B   0x80013ee4, onDllLoad
    PATCH_B   0x80013e78, onDllUnload
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE, 0x80 # how much to reserve
    .set SP_LR_SAVE, 0x10
    .set SP_GPR_SAVE,0x18

entry: # called as soon as our patch is loaded.
    blr # nothing to do here

onDllLoad:
    stwu   r1,  -STACK_SIZE(r1) # get some stack space
    stmw   r3,  SP_GPR_SAVE(r1)
    mflr   r29
    stw    r29, SP_LR_SAVE(r1)

    bl .onload_getpc
    .onload_getpc: mflr r31

    LOAD   r8,  dllRefCount # if already loaded, don't print.
    slwi   r9,  r3,  1
    lhzx   r7,  r8,  r9
    cmpwi  r7,  0
    bne    .onLoad_alreadyLoaded

    mr     r6, r4  # callback param
    mr     r5, r3  # ID
    mr     r4, r29 # lr
    addi   r3, r31, (.msg_load - .onload_getpc)@l
    CALL   OSReport

.onLoad_alreadyLoaded:
    lwz    r29, SP_LR_SAVE(r1)
    mtlr   r29 # restore LR
    lmw    r3,  SP_GPR_SAVE(r1)
    addi   r1, r1, STACK_SIZE # restore stack ptr
    rlwinm r0,r3,0x0,0x10,0x1f # replaced
    JUMP   0x80013ee8, r31 # return to original code

onDllUnload:
    stwu   r1,  -STACK_SIZE(r1) # get some stack space
    stmw   r3,  SP_GPR_SAVE(r1)
    mflr   r29
    stw    r29, SP_LR_SAVE(r1)

    bl .onunload_getpc
    .onunload_getpc: mflr r31

    LOAD   r8,  dllRefCount # if still loaded, don't print.
    slwi   r9,  r5,  1
    lhzx   r7,  r8,  r9
    cmpwi  r7,  1
    bne    .onUnload_stillLoaded

    # r5 = Idx
    mr     r6, r3  # DLL*
    mr     r4, r29 # lr
    addi   r3, r31, (.msg_unload - .onunload_getpc)@l
    CALL   OSReport

.onUnload_stillLoaded:
    lwz    r29, SP_LR_SAVE(r1)
    mtlr   r29 # restore LR
    lmw    r3,  SP_GPR_SAVE(r1)
    addi   r1, r1, STACK_SIZE # restore stack ptr
    lis    r4, 0x8034 # replaced
    JUMP   0x80013e7c, r5 # return to original code

.msg_load:   .string "%08X load   DLL %04X param %08X rc=%d"
.msg_unload: .string "%08X unload DLL %04X addr  %08X rc=%d"
