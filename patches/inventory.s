# inventory patch:
# replaces Fuel Cells in the inventory with a sub-inventory
# containing rarely-used items so they don't clog up the
# main inventory.
.text
.include "common.s"

# define patches
patchList:
    PATCH_ID        "Inventy" # must be 7 chars
    PATCH_MAIN_LOOP mainLoop
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,  0xC0 # how much to reserve
    .set SP_LR_SAVE,  0xC4
    .set SP_FLOAT_TMP,0x18 # temporary storage for float conversion (8 bytes)
    .set SP_GPR_SAVE, 0x20

entry: # called as soon as our patch is loaded.
    blr

mainLoop: # called from main loop. r3 = mainLoop
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r4
    stw     r4,  SP_LR_SAVE(r1)
    mr      r14, r3

    # is the menu open?
    lbz     r3, (isVisible - mainLoop)(r14)
    cmpwi   r3, 0
    beq     .end

    bl      drawMenu

    #LOADWH  r15, controllerStates
    #LOADBL2 r3, controllerStates+2, r9 # stick X
    #LOADBL2 r4, controllerStates+3, r9 # stick Y
    #extsb   r3, r3 # sign extend (PPC Y U NO LBA OPCODE!?)
    #extsb   r4, r4
    #LOADHL2 r6, controllerStates, r9 # buttons
    #LOADBL2 r5, controllerStates+6, r9 # L
    #LOADBL2 r6, controllerStates+7, r9 # R
    #neg     r5, r5
    #add     r5, r5, r6 # r5 = R - L
    # 816a9d04


.end:
    lwz    r29, SP_LR_SAVE(r1)
    mtlr   r29 # restore LR
    lmw    r3,  SP_GPR_SAVE(r1)
    addi   r1, r1, STACK_SIZE # restore stack ptr
    blr


drawMenu:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r4
    stw     r4,  SP_LR_SAVE(r1)

    li      r3,  20 # X
    li      r4,  20 # Y
    li      r5, 600 # W
    li      r6, 440 # H
    bl      menuDrawBoxOpaque
    li      r3,  20 # X
    li      r4,  20 # Y
    li      r5, 600 # W
    li      r6, 440 # H
    bl      menuDrawBoxOpaque

    LOAD    r16, 0x8031b100 # item ptr
    li      r17, 0 # item idx
    li      r21, 0 # grid pos
.drawMenu_nextItem:
    slwi    r19, r17, 4
    add     r19, r16, r19

    # have we collected this item?
    lha     r3,  0x00(r19) # itemBit
    cmpwi   r3,  0
    blt     .drawMenu_checkedHaveBit
    CALL    mainGetBit
    cmpwi   r3,  0
    beq     .drawMenu_skipItem
.drawMenu_checkedHaveBit:

    # have we used up this item?
    lha     r3,  0x02(r19) # hideBit
    cmpwi   r3,  0
    blt     .drawMenu_checkedHideBit
    CALL    mainGetBit
    cmpwi   r3,  0
    bne     .drawMenu_skipItem
.drawMenu_checkedHideBit:

    # get texture
    lhz     r3,  0x06(r19)
    CALL    0x80054d54 # tryLoadTextureFile
    cmpwi   r3,  0
    beq     .drawMenu_skipItem
    mr      r18, r3

    # set item draw position
    andi.   r3,  r21, 0xF8
    slwi    r3,  r3,  3
    addi    r3,  r3,  32
    bl      intToFloat # Y
    fmr     f2,  f1
    andi.   r3,  r21, 7
    slwi    r3,  r3,  6
    addi    r3,  r3,  40
    bl      intToFloat # X
    mr      r3,  r18
    li      r4,  0xFF # opacity
    li      r5,  0x0100 # scale
    CALL    0x8007719c # draw texture
    mr      r3,  r18
    CALL    0x80054308 # free texture

    lbz     r3,  (cursorPos - mainLoop)(r14)
    cmpw    r3,  r21
    bne     .drawMenu_notSelected

    # draw the cursor
    # XXX do not draw the interior
    andi.   r3,  r21, 0xF8
    slwi    r3,  r3,  3
    addi    r4,  r3,  24 # Y
    andi.   r3,  r21, 7
    slwi    r3,  r3,  6
    addi    r3,  r3,  32 # X
    li      r5,  64   # W
    li      r6,  64   # H
    li      r20, 0x80 # opacity
    bl      menuDrawBox

    # draw the name
    li      r3,  3
    li      r4,  3
    CALL    0x80019b1c # magic to give us the real text
    lhz     r3,  0x0C(r19) # text ID
    CALL    0x80019444 # gameTextGetString
    li      r4,  0x0A # box ID
    CALL    0x80015e84 # gameTextRenderStr


.drawMenu_notSelected:
    addi    r21, r21, 1 # next grid pos
.drawMenu_skipItem:
    addi    r17, r17, 1
    cmpwi   r17, 0x3D
    blt     .drawMenu_nextItem

.drawMenu_end:
    lwz    r29, SP_LR_SAVE(r1)
    mtlr   r29 # restore LR
    lmw    r3,  SP_GPR_SAVE(r1)
    addi   r1, r1, STACK_SIZE # restore stack ptr
    blr

# used by menuDrawBox
menuEndSub:
    # common end code for menu subroutines.
    lwz  r0, SP_LR_SAVE(r1)
    mtlr r0 # restore LR
    lmw  r13, SP_GPR_SAVE(r1)
    addi r1, r1, STACK_SIZE # restore stack ptr
    blr
.include "pda/util.s"
.include "pda/drawbox.s"

isVisible: .byte 0 # is inventory menu open?
cursorPos: .byte 4 # selected slot
