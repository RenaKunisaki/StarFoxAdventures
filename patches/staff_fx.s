# staff FX patch:
# makes the staff look better.
.text
.include "common.s"

# define patches
patchList:
    PATCH_ID  "staffFX" # must be 7 chars
    PATCH_B 0x8016de60, doSwipe # bl gxBeginVtxs
    #PATCH_WORD 0x8016dd9c, 0x38a00003 # change color mode
    PATCH_WORD 0x8016dd74, 0x60000000 # allow red to show
    # XXX this isn't very good as it leaves whatever other
    # texture active. should find a way to disable it or
    # point to an all-white texture.

    # make the grow/shrink animation run even if the staff was
    # "instantly" taken out/put away
    PATCH_WORD 0x8016e758, 0x60000000
    PATCH_WORD 0x8016e71c, 0x60000000

    # slow down that animation a bit so you can actually see it
    PATCH_WORD 0x803e3320, 0x3dcccccd #  0.15 ->  0.10
    PATCH_WORD 0x803e3324, 0xbdcccccd # -0.15 -> -0.10

    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
.set STACK_SIZE,0x100 # how much to reserve
.set SP_LR_SAVE,0x104 # this is what the game does
.set SP_H,0x10
.set SP_GPR_SAVE,0x20

entry: # called as soon as our patch is loaded.
    blr # nothing to do here

doSwipe:
    # copy of the code that writes the vertices for each
    # segment of the staff's swipe effect.
    # modified to give it a rainbow hue.
    stwu     r1, -STACK_SIZE(r1) # get some stack space
    mflr     r9
    stw      r9, SP_LR_SAVE(r1)
    stmw     r13, SP_GPR_SAVE(r1)

    CALL     gxBeginVtxs

    # simplified HSV-to-RGB conversion
    # SP_H stores the hue (0 to 7)
    # S, V are fixed
    lbz      r3, SP_H(r1)
    addi     r3, r3, 1
    andi.    r3, r3, 0x1F
    stb      r3, SP_H(r1)
    srwi.    r3, r3, 1 # slow down cycling

    addi     r4, r3, 2 # g
    addi     r5, r3, 8 # b
    andi.    r4, r4, 0xF
    andi.    r5, r5, 0xF

    mflr r7
    bl       .getpc
    .getpc:  mflr r6
    mtlr     r7
    addi     r6, r6, .colorTbl - .getpc
    lbzx     r3, r3, r6
    lbzx     r4, r4, r6
    lbzx     r5, r5, r6
    #fmr      f30, f29
    #fmr      f31, f29

    # repeat most of the original code here...
    # vtx 1
    lfs      f1,0x8(r28)
    lfs      f0,-0x6404(r13) #=>playerMapOffsetZ
    fsubs    f3,f1,f0
    lfs      f2,0x4(r28)
    lfs      f1,0x0(r28)
    lfs      f0,-0x6408(r13) #=>playerMapOffsetX
    fsubs    f0,f1,f0
    stfs     f0,-0x8000(r29) # X
    stfs     f2,-0x8000(r29) # Y
    stfs     f3,-0x8000(r29) # Z
    lha      r0,0x10(r28) # alpha
    rlwinm   r0,r0,0x0,0x18,0x1f
    stb      r3,-0x8000(r29)  # R
    stb      r4,-0x8000(r29)  # G
    stb      r5,-0x8000(r29)  # B
    stb      r0,-0x8000(r29)  # A
    stfs     f29,-0x8000(r29) # S
    stfs     f30,-0x8000(r29) # T

    # vtx 2
    lfs      f1,0x1c(r28)
    lfs      f0,-0x6404(r13) #=>playerMapOffsetZ
    fsubs    f3,f1,f0
    lfs      f2,0x18(r28)
    lfs      f1,0x14(r28)
    lfs      f0,-0x6408(r13) #=>playerMapOffsetX
    fsubs    f0,f1,f0
    stfs     f0,-0x8000(r29) # X
    stfs     f2,-0x8000(r29) # Y
    stfs     f3,-0x8000(r29) # Z
    lha      r0,0x24(r28)
    rlwinm   r0,r0,0x0,0x18,0x1f
    stb      r3,-0x8000(r29)  # R
    stb      r4,-0x8000(r29)  # G
    stb      r5,-0x8000(r29)  # B
    stb      r0,-0x8000(r29)  # A
    stfs     f29,-0x8000(r29) # S
    stfs     f31,-0x8000(r29) # T

    # vtx 3
    lfs      f1,0x44(r28)
    lfs      f0,-0x6404(r13) #=>playerMapOffsetZ
    fsubs    f3,f1,f0
    lfs      f2,0x40(r28)
    lfs      f1,0x3c(r28)
    lfs      f0,-0x6408(r13) #=>playerMapOffsetX
    fsubs    f0,f1,f0
    stfs     f0,-0x8000(r29) # X
    stfs     f2,-0x8000(r29) # Y
    stfs     f3,-0x8000(r29) # Z
    lha      r0,0x4c(r28)
    rlwinm   r0,r0,0x0,0x18,0x1f
    stb      r3,-0x8000(r29)  # R
    stb      r4,-0x8000(r29)  # G
    stb      r5,-0x8000(r29)  # B
    stb      r0,-0x8000(r29)  # A
    stfs     f29,-0x8000(r29) # S
    stfs     f31,-0x8000(r29) # T

    # vtx 4
    lfs      f1,0x30(r28)
    lfs      f0,-0x6404(r13) #=>playerMapOffsetZ
    fsubs    f3,f1,f0
    lfs      f2,0x2c(r28)
    lfs      f1,0x28(r28)
    lfs      f0,-0x6408(r13) #=>playerMapOffsetX
    fsubs    f0,f1,f0
    stfs     f0,-0x8000(r29) # X
    stfs     f2,-0x8000(r29) # Y
    stfs     f3,-0x8000(r29) # Z
    lha      r0,0x38(r28)
    rlwinm   r0,r0,0x0,0x18,0x1f
    stb      r3,-0x8000(r29)  # R
    stb      r4,-0x8000(r29)  # G
    stb      r5,-0x8000(r29)  # B
    stb      r0,-0x8000(r29)  # A
    stfs     f29,-0x8000(r29) # S
    stfs     f30,-0x8000(r29) # T

.end:
    lwz      r0, SP_LR_SAVE(r1)
    mtlr     r0 # restore LR
    lmw      r13, SP_GPR_SAVE(r1)
    addi     r1, r1, STACK_SIZE # restore stack ptr
    JUMP     0x8016df88, r0

.colorTbl:
    .byte 0xFF, 0xC0, 0xA0, 0x80, 0x60, 0x40, 0x20, 0x00
    .byte 0x00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xFF
