# alloc debug patch:
# logs allocations and flags any that are absurd.
# also, changes the "tag" of each allocation to the address of the
# function that did the allocation.
# hopefully this won't hurt anything.
.text
.include "common.s"
.include "globals.s"
.set ENABLE_LOG,0

# define patches
patchList:
    PATCH_ID        "alloc  " # must be 7 chars
    PATCH_B         0x80023D00, onAlloc
    PATCH_B         0x80023B0C, onAllocFail
    PATCH_MAIN_LOOP mainLoop
    PATCH_END       PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,0x100 # how much to reserve
    .set SP_LR_SAVE,0x104
    .set SP_STR_BUF, 0x20
    .set SP_GPR_SAVE,0x40

entry: # called as soon as our patch is loaded.
    blr # nothing to do here


mainLoop:
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r9
    stw     r9,  SP_LR_SAVE(r1)
    mr      r14, r3

    # check memory usage
    LOAD    r16, 0x803406A0 # heap 0
    li      r15, 0 # idx
    li      r5,  0 # total bytes
    li      r6,  0 # total blocks
    li      r7,  0 # used bytes
    li      r8,  0 # used blocks
.nextHeap:
    lwz     r9,  0x0C(r16) # total bytes
    add     r5,  r5,  r9
    lwz     r9,  0x00(r16) # total blocks
    add     r6,  r6,  r9
    lwz     r9,  0x10(r16) # used bytes
    add     r7,  r7,  r9
    lwz     r9,  0x04(r16) # used blocks
    add     r8,  r8,  r9
    addi    r16, r16, 0x14 # heap entry size
    addi    r15, r15, 1
    cmpwi   r15, 4
    bne     .nextHeap

    mulli   r7,  r7,  100
    divwu   r4,  r7,  r5 # used bytes percent
    mulli   r8,  r8,  100
    divwu   r5,  r8,  r6 # used blocks percent

    # trigger emergency free if 95% of bytes or blocks are used.
    cmpwi   r4,  95
    bge     .emergencyFree
    cmpwi   r5,  95
    bge     .emergencyFree

end:
    lwz     r9, SP_LR_SAVE(r1)
    mtlr    r9 # restore LR
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    blr

.emergencyFree:
    addi    r3,  r14, s_emergFree - mainLoop
    CALL    OSReport

.emergencyFreeDo:
    LOADWH  r15, loadedObjects
    LOADWL2 r16, numLoadedObjs, r15
    LOADWL2 r15, loadedObjects, r15
    cmpwi   r16, 0
    beq     .end # no objects
    li      r17, 0 # index

.emergFreeNextObj:
    slwi    r4,  r17, 2
    lwzx    r18, r4,  r15   # r18 = ObjInstance*

    lhz     r3,  0x46(r18) # defNo
    addi    r5,  r14, (emergFreeObjDefs - mainLoop) - 2
.emergFreeNextObjDef:
    lhau    r6,  2(r5)
    cmpwi   r6,  -1
    beq     .emergFreeNotThisDef
    cmpw    r6,  r3 # is this defNo on the list?
    beq     .doFree
    b       .emergFreeNextObjDef

.emergFreeNotThisDef:
    # this defNo isn't on the list, but what about the DLL?
    lwz     r7,  0x50(r18) # ObjectFileStruct*
    lhz     r7,  0x50(r7) # DLL ID
    # r5 already points to emergFreeDlls - 2
.emergFreeNextDll:
    # reversing these checks saves an instruction but also causes us to
    # free objects with DLL -1 which we definitely don't want
    lhau    r6,  2(r5)
    cmpwi   r6,  -1
    beq     .emergFreeNotThisObj
    cmpw    r6,  r7
    beq     .doFree
    b       .emergFreeNextDll

.emergFreeNotThisObj:
    # this object doesn't match, try next one
    addi    r17, r17, 1
    cmpw    r17, r16
    bne     .emergFreeNextObj

    # we didn't free anything...
    addi    r3,  r14, s_noFree - mainLoop
    CALL    OSReport
    b       .end

.doFree:
    # only free one object at a time.
    # if we're still out of memory we'll free another one again next frame.
    # this avoids freeing more than is needed, and dealing with the object
    # list changing while we're iterating it.

    # manually copy the name since it's not terminated
    addi    r3,  r1,  SP_STR_BUF
    lwz     r17, 0x50(r18) # ObjectFileStruct*
    addi    r4,  r17, 0x91 # name
    li      r5,  11
    CALL    strncpy

    addi    r7,  r1,  SP_STR_BUF
    li      r4,  0
    stb     r4,  11(r7) # strncpy doesn't do this if src is long

    mr      r4,  r18
    lha     r5,  0x46(r4)
    lhz     r6,  0x50(r17) # DLL ID
    addi    r3,  r14, s_doFree - mainLoop
    CALL    OSReport
    mr      r3,  r18
    CALL    objFree
    b       .end

emergFreeObjDefs:
    .short 0x0025 # ModelsWithT
    .short 0x0175 # TrickyExcla
    .short 0x017B # TrickyFood
    .short 0x017C # TrickyQuest
    .short 0x063C # groundquake
    .short 0x0666 # KooshyHill
    .short 0x066A # Backpack
    .short 0xFFFF
emergFreeDlls:
    #.short 0x00C6 # AnimatedObj
    .short 0x00CC # ChukChuk
    .short 0x00CD # IceBall
    .short 0x00CF # CannonClaw
    .short 0x00D0 # Grimble
    .short 0x00D1 # Tumbleweed
    .short 0x00D2 # Tumbleweed1
    .short 0x00D5 # Kaldachom
    .short 0x00D6 # KaldaChompMe
    .short 0x00D7 # KaldachomSp
    .short 0x00D8 # PinPonSpike
    .short 0x00D9 # Pollen
    .short 0x00DA # PollenFragm
    .short 0x00DC # MikaBombSha
    .short 0x00DF # Hagabon
    .short 0x00E0 # SwarmBaddie
    .short 0x00E1 # WispBaddie
    .short 0x00E3 # Fireball
    .short 0x011A # Decoration11A
    .short 0x0128 # KT_Torch
    .short 0x0262 # DrakorMissi
    .short 0x02AD # SoftBody (flowers)
    .short 0xFFFF


onAlloc:
    # patch into allocTagged
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r9
    stw     r9,  SP_LR_SAVE(r1)

    bl .getpc
    .getpc:
        mflr r31

.if ENABLE_LOG
    addi r3, r31, (.msg - .getpc)@l
    mr   r4, r9  # lr
    mr   r5, r28 # size
    mr   r6, r29 # tag
    mr   r7, r30 # name
    CALL OSReport
.endif

    # check size
    srwi  r4, r28, 16
    cmpwi r4, 0x0180
    blt   .ok

    # trigger a BSOD
    li    r3, 0
    stb   r3, 0x0BAD(r3)

.ok:
    lwz     r9,  SP_LR_SAVE(r1)
    mtlr    r9 # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    lwz     r29, SP_LR_SAVE(r1) # replace tag with lr (not name, because it isn't stored)
    addi    r1,  r1, STACK_SIZE # restore stack ptr

    li      r0,  1 # replaced
    JUMP    0x80023d04, r31 # return to original code


onAllocFail:
    # replaces an OSReport call logging alloc failures
    # r3: str, r4: name, r5: region, r6: tag, r7: size, r8: largest
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r11
    stw     r11, SP_LR_SAVE(r1)
    CALL    OSReport # replaced

    # also log it
    LOADW   r3,  PATCH_STATE_PTR
    lbz     r4,  ALLOC_FAIL_POS(r3) # get buffer pos
    slwi    r4,  r4,  3 # multiply by 8
    addi    r4,  r4,  ALLOC_FAIL_LOG # to buffer start
    lwz     r5,  (SP_GPR_SAVE+(3*4))(r1) # retrieve stored r6 (tag)
    lwz     r6,  (SP_GPR_SAVE+(4*4))(r1) # retrieve stored r7 (size)
    stwx    r5,  r4,  r3 # store tag
    addi    r4,  r4,  4
    stwx    r6,  r4,  r3 # store size

    # increment log pointer
    lbz     r4,  ALLOC_FAIL_POS(r3)
    addi    r4,  r4,  1
    andi.   r4,  r4,  (ALLOC_FAIL_MAX - 1)
    stb     r4,  ALLOC_FAIL_POS(r3)

    # finally, do an emergency free.
    # XXX we should ideally try to free enough to cover this allocation,
    # instead of just a single object.
    b       .emergencyFreeDo

.if ENABLE_LOG
    .msg: .string "%08X alloc %08X %08X %s"
.endif

s_emergFree: .string "Mem: %d%% bytes/%d%% blocks used, triggering emergency free"
s_noFree:    .string "Failed to free anything!"
s_doFree:    .string "Free obj 0x%08X (def 0x%04X DLL 0x%04X): %s"
