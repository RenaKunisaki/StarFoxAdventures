# alloc debug patch:
# improves allocator in various ways:
# - emergency free if out of memory
# - log alloc failures, and optionally all allocations
# - change alloc tag to return address for better tracking
.text
.include "common.s"
.include "globals.s"
.set ENABLE_LOG,0
.set ALLOC_DEBUG,0

# define patches
patchList:
    PATCH_ID        "alloc  " # must be 7 chars
    PATCH_B         0x80023D00, onAlloc
    #PATCH_B         0x80023B0C, onAllocFail
    PATCH_END       PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,0x100 # how much to reserve
    .set SP_LR_SAVE,0x104
    .set SP_STR_BUF, 0x20
    .set SP_GPR_SAVE,0x40

entry: # called as soon as our patch is loaded.
    blr # nothing to do here


onAlloc:
    # patch into allocTagged
    # r26: (our var) attempt count
    # r27: (our var) which heaps to use
    # r28: size
    # r29: tag
    # r30: name
    # r31: (our var) PC reference
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r4,  SP_GPR_SAVE(r1)
    mflr    r9
    stw     r9,  SP_LR_SAVE(r1)

    bl .getpc
    .getpc:
        mflr r31

.if ENABLE_LOG
    addi    r3,  r31, (.msg - .getpc)@l
    mr      r4,  r9  # lr
    mr      r5,  r28 # size
    mr      r6,  r29 # tag
    mr      r7,  r30 # name
    CALL    OSReport
.endif

    # check size
    srwi    r4,  r28, 16
    andi.   r4,  r4,  0xFFFF # mask off high bits - ensure positive
    cmpwi   r4,  0x0180
    blt     .doAlloc_ok

    # trigger a BSOD if size is insane
    li      r3,  0
    stb     r3,  0x0BAD(r3)

.doAlloc_ok: # size is sane
    lwz     r29, SP_LR_SAVE(r1) # replace tag with lr (not name, because it isn't stored)
    li      r26, 100 # attempt count

.doAlloc_retry:
    # duplicate some of the game logic regarding which heaps to try in which order.
    li      r27, 0xF021@l # 1, 2, 0, -1, in reverse order

    lwz     r0,  -0x7dac(r13) # bOnlyUseHeaps1and2
    cmpwi   r0,  1 # game checks for 1 specifically
    beq     .doAlloc_only1and2

    lwz     r0,  -0x66d8(r13) # bOnlyUseHeap3
    cmpwi   r0,  0 # here it checks for != 0
    bne     .doAlloc_only3

    cmpwi   r28, 0x3000 # if size >= 0x3000, use heaps 0, 1
    bge     .doAlloc_only0and1

    cmpwi   r28, 0x0400 # if size >= 0x0400, use heaps 1, 2, 0
    bge     .tryNextHeap # r27 is already set.

    # size < 0x400 - use heaps 2, 1, 0
    li      r27,  0xF012@l # 2, 1, 0, -1, in reverse order
    b       .tryNextHeap

.doAlloc_only0and1:
    li      r27, 0xFF10@l # 0, 1, -1, -1, in reverse order
    b       .tryNextHeap

.doAlloc_only1and2:
    li      r27, 0xFF21@l # 1, 2, -1, -1, in reverse order
    b       .tryNextHeap

.doAlloc_only3:
    li      r27, 0xFFF3@l # 3, -1, -1, -1, in reverse order

.tryNextHeap: # try to alloc from the heaps specified by r27
.if ALLOC_DEBUG
    andi.   r4,  r27, 0x000F # get heap idx
    mr      r5,  r27
    mr      r6,  r28
    mr      r7,  r26
    addi    r3,  r31, s_tryHeap - .getpc
    CALL    OSReport
.endif # ALLOC_DEBUG

    andi.   r3,  r27, 0x000F # get heap idx
    srwi    r27, r27, 4 # next heap idx
    cmpwi   r3,  0x000F
    beq     .doAlloc_fail # no more heaps to try
    mr      r4,  r28 # size
    mr      r5,  r29 # tag
    mr      r6,  r30 # name
    CALL    heapAlloc
    cmpwi   r3,  0
    bne     .doAlloc_success
    b       .tryNextHeap

.doAlloc_giveUp:
    # we've tried too many times... nothing more we can do,
    # except to log the failure.
    mr      r4,  r28 # size
    mr      r5,  r29 # tag
    addi    r3,  r31, s_allocFail - .getpc
    CALL    OSReport

    LOADW   r3,  PATCH_STATE_PTR
    lbz     r4,  ALLOC_FAIL_POS(r3) # get buffer pos
    slwi    r4,  r4,  3 # multiply by 8
    addi    r4,  r4,  ALLOC_FAIL_LOG # to buffer start
    mr      r5,  r29  # tag
    mr      r6,  r28 # size
    stwx    r5,  r4,  r3 # store tag
    addi    r4,  r4,  4
    stwx    r6,  r4,  r3 # store size

    # increment log pointer
    lbz     r4,  ALLOC_FAIL_POS(r3)
    addi    r4,  r4,  1
    andi.   r4,  r4,  (ALLOC_FAIL_MAX - 1)
    stb     r4,  ALLOC_FAIL_POS(r3)

    li      r3,  0 # return null

.doAlloc_success:
.if ALLOC_DEBUG
    mr      r27, r3
    mr      r4,  r3
    addi    r3,  r31, s_allocSuccess - .getpc
    CALL    OSReport
    mr      r3,  r27
.endif # ALLOC_DEBUG

    lwz     r9,  SP_LR_SAVE(r1)
    mtlr    r9 # restore LR
    lmw     r4,  SP_GPR_SAVE(r1)
    #lwz     r29, SP_LR_SAVE(r1) # replace tag with lr (not name, because it isn't stored)
    addi    r1,  r1, STACK_SIZE # restore stack ptr
    JUMP    0x80023eb4, r31 # return to end of original function

.doAlloc_fail: # we didn't manage to alloc from anywhere.
    subi    r26, r26, 1
    cmpwi   r26, 0
    beq     .doAlloc_giveUp

emergencyFree:
    mr      r4,  r28 # size
    mr      r5,  r29 # tag
    mr      r6,  r26 # retry count
    addi    r3,  r31, s_emergFree - .getpc
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

    lhz     r3,  0x44(r18) # catId
    addi    r5,  r31, (emergFreeCats - .getpc) - 1
.emergFreeNextCat:
    lbzu    r6,  1(r5)
    cmpwi   r6,  0xFF
    beq     .emergFreeNotThisObj
    cmpw    r6,  r3 # is this category on the list?
    beq     .doFree
    b       .emergFreeNextCat

.emergFreeNotThisObj:
    # this object doesn't match, try next one
    addi    r17, r17, 1
    cmpw    r17, r16
    bne     .emergFreeNextObj

    # we didn't free anything...
    addi    r3,  r31, s_noFree - .getpc
    CALL    OSReport
    # let's try dumping all expgfx.
    CALL    expgfxRemoveAll
    b       .doAlloc_retry

.doFree:
    # only free one object at a time.
    # if we're still out of memory we'll free another one again next time.
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
    addi    r3,  r31, s_doFree - .getpc
    CALL    OSReport
    mr      r3,  r18
    CALL    objFree
    b       .doAlloc_retry


.if ENABLE_LOG
    .msg: .string "%08X alloc %08X %08X %s"
.endif

s_emergFree: .string "Alloc failed, trying emergency free (size %d tag %08X retry=%d)"
s_noFree:    .string "Failed to free anything!"
s_doFree:    .string "Free obj 0x%08X (def 0x%04X DLL 0x%04X): %s"
s_allocFail: .string "ALLOC FAILURE size %d tag %08X"
.if ALLOC_DEBUG
    s_tryHeap:      .string "Try heap %d (%04X) size %d retry=%d"
    s_allocSuccess: .string "Alloc success, %08X"
.endif

emergFreeCats: # object category IDs
    # ordered roughly from least to most important as well as least to
    # most noticeable
    .byte 0x7F # Decorative
    .byte 0x69 # CurveFish
    .byte 0x43 # LevelName
    .byte 0x83 # Lightning
    .byte 0x35 # CCriverflow
    .byte 0x5C # sfxPlayer
    .byte 0x08 # InfoPoint
    .byte 0x79 # Light
    .byte 0x7B # ProjectedLight
    .byte 0x73 # WaterFallSpray
    .byte 0x3C # SidekickBall
    .byte 0x1D # Torch
    .byte 0x32 # CampFire
    .byte 0x3D # CFPerch
    .byte 0x45 # DIMLavaBall
    .byte 0x46 # DIMSnowBall
    .byte 0x24 # Fireball
    .byte 0x28 # Falling Rock
    .byte 0x2A # Enemy Mushroom
    .byte 0x1C # Baddie
    .byte 0x38 # some doors
    .byte 0x53 # other doors
    .byte 0x59 # WM_Column
    .byte 0x6A # Tree
    .byte 0x29 # Edible Mushroom
    .byte 0x65 # MagicPlant
    .byte 0x06 # Collectible
    .byte 0x63 # Exploded, Explodable
    .byte 0x7C # ArwingLevelObj
    .byte 0x44 # SH_thorntail
    .byte 0x26 # Mammoth
    .byte 0x40 # CloudRunner
    .byte 0x80 # DeathGas
    .byte 0xFF # end of list
