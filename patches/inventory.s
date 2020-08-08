# inventory patch:
# replaces Fuel Cells in the inventory with a sub-inventory
# containing rarely-used items so they don't clog up the
# main inventory.
#
# This almost works. For some items it works right away, but for
# others, it only "selects" them and you need to press A again
# to actually use them (the C icon will show up over the target).
# This is made more problematic by the fact that the A press that
# opens the menu also speaks to NPCs.
.text
.include "common.s"
.include "globals.s"

# define patches
patchList:
    PATCH_ID        "Invntry" # must be 7 chars
    PATCH_MAIN_LOOP mainLoop
    PATCH_PAD_HOOK  padHook
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,  0xC0 # how much to reserve
    .set SP_LR_SAVE,  0xC4
    .set SP_FLOAT_TMP,0x18 # temporary storage for float conversion (8 bytes)
    .set SP_GPR_SAVE, 0x20

entry: # called as soon as our patch is loaded.
    blr

padHook: # called from pad read. r3 = padHook, r4 = pad states ptr
    # crazy enough, this seems to be the only reliable way to actually
    # "use" an item: actually simulate some A presses.
    # all the various "use item" routines actually just check for buttons.
    # XXX find a way to prevent the player pulling out their staff or
    # talking to NPCs or etc if we select an unusable item.
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r5
    stw     r5,  SP_LR_SAVE(r1)
    mr      r14, r3

    lbz     r3, (isVisible - padHook)(r14)
    cmpwi   r3, 0
    bne     .end

    lbz     r5,  (cMenuTimer - padHook)(r14)
    andi.   r5,  r5,  1
    cmpwi   r5,  0
    beq     .end

    li      r5,  PAD_BUTTON_A
    sth     r5,  0x00(r4)

    addi    r3,  r14, s_mash - padHook
    CALL    debugPrintf
    #CALL    OSReport

    b       .end


mainLoop: # called from main loop. r3 = mainLoop
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r4
    stw     r4,  SP_LR_SAVE(r1)
    mr      r14, r3


    LOADWH  r16, controllerStates

    # is the menu open?
    lbz     r3, (isVisible - mainLoop)(r14)
    cmpwi   r3, 0
    bne     .mainLoop_menuIsOpen

    # are we attempting to use a fuel cell?
    LOADWH  r4,  0x803dca68
    LOADWL2 r3,  0x803dca68, r4 # dllFuncs
    lwz     r3,  0(r3) # UI:funcs
    lwz     r12, 0x20(r3) # UI:isItemBeingused
    mtspr   CTR, r12
    li      r3,  0x3F5 # NumFuelCells
    bctrl
    cmpwi   r3,  0
    beq     .mainLoop_menuNotOpen

    # we are!
    stb     r3, (isVisible - mainLoop)(r14)
    # XXX on open, check cursor pos is valid, in case we lost an item.

    # don't immediately respond to A press
    li      r3,  3
    stb     r3,  (moveTimer - mainLoop)(r14)
    stb     r3,  (cMenuTimer - mainLoop)(r14)
    li      r3,  5
    stb     r3,  (openTimer - mainLoop)(r14)

    # don't have other things respond to it either
    li      r3,  0 # pad
    li      r4,  PAD_BUTTON_A|PAD_BUTTON_Y
    CALL    0x80014b3c # buttonDisable

.mainLoop_menuIsOpen:
    li     r4,  1
    LOADWH r5,  timeStop
    STOREB r4,  timeStop, r5 # stops all objects
    STOREB r4,  pauseDisabled, r5 # inhibit pause menu
    li     r4,  0
    STOREB r4,  cMenuEnabled, r5 # stop C menu clearing item flags
    # inhibit game timer
    LOAD   r5,  0x800140BC # gameTimerRun
    LOAD   r4,  0x4E800020 # blr
    stw    r4,  0(r5)
    li     r4,  0
    icbi   r4,  r5 # flush instruction cache

    bl     drawMenu

    lbz    r3,  (openTimer - mainLoop)(r14)
    cmpwi  r3,  0
    beq    .mainLoop_notJustOpened
    subi   r4,  r3, 1
    stb    r4,  (openTimer - mainLoop)(r14)
    addi   r3,  r14, s_openTimer - mainLoop
    CALL   debugPrintf
    b      .end


.mainLoop_notJustOpened:
    bl     menuDoInput
    cmpwi  r3,  0
    beq    .closeMenu

.end:
    lwz    r29, SP_LR_SAVE(r1)
    mtlr   r29 # restore LR
    lmw    r3,  SP_GPR_SAVE(r1)
    addi   r1,  r1, STACK_SIZE # restore stack ptr
    blr

.mainLoop_menuNotOpen:
    lbz    r4,  (cMenuTimer - mainLoop)(r14)
    cmpwi  r4,  0
    beq    .end
    subi   r4,  r4,  1
    stb    r4,  (cMenuTimer - mainLoop)(r14)
    li     r5,  0
    LOADWH r6,  cMenuEnabled
    STOREB r5,  cMenuEnabled, r6 # stop C menu clearing item flags
    addi   r3,  r14, s_cTimer - mainLoop
    CALL   debugPrintf
    #li      r3,  0 # pad
    #li      r4,  PAD_BUTTON_A|PAD_BUTTON_Y
    #CALL    0x80014b3c # buttonDisable
    #li      r4,  PAD_BUTTON_A
    #STOREH  r4,  controllerStates, r16
    #STOREH  r4,  controllerStates+0x30, r16
    #LOADWH  r3,  0x803dc91c
    #STOREH  r4,  0x803dc91c, r3
    #LOADWH  r3,  0x803398d0
    #STOREW  r4,  0x803398d0, r3
    #STOREW  r4,  0x803398e0, r3
    #STOREW  r4,  0x803398c0, r3
    lhz     r3,  (curItemBit - mainLoop)(r14)
    LOADWH  r4,  0x803dd8c2 # selectedItem
    STOREH  r3,  0x803dd8c2, r4 # itemBeingUsed
    STOREH  r3,  0x803dd8c0, r4 # cMenuSelectedItemBit
    li      r3,  0
    STOREB  r3,  0x803dd795, r4 # cMenuOpen
    li      r3,  0 # XXX item->fieldF
    STOREB  r3,  0x803dd8b8, r4 # isItemBeingUsed
    STOREW  r3,  0x803dd8a8, r4
    STOREB  r3,  0x803dd7d4, r4
    li      r3,  1
    STOREH  r3,  0x803dd8d6, r4
    b      .end

.closeMenu: # close the menu
    li     r3, 0
    stb    r3, (isVisible - mainLoop)(r14)
    LOADWH r5, timeStop
    STOREB r3, timeStop, r5 # unpause game
    LOADWH r5, pauseDisabled
    STOREB r3, pauseDisabled, r5 # allow pause menu

    # restore game timer
    LOAD   r5, 0x800140BC # gameTimerRun
    LOAD   r4, 0x9421FFC0 # original opcode
    stw    r4, 0(r5)
    li     r4, 0
    icbi   r4, r5 # flush instruction cache
    b      .end


menuDoInput: # return r3 = 1 to keep open, 0 to close
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stmw    r4,  SP_GPR_SAVE(r1) # not r3
    mflr    r4
    stw     r4,  SP_LR_SAVE(r1)

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

    # get input
    LOADHL2 r6,  controllerStates,   r16 # buttons
    LOADBL2 r7,  controllerStates+2, r16 # stick x
    extsb   r7,  r7
    LOADBL2 r8,  controllerStates+3, r16 # stick y
    extsb   r8,  r8

    # check move timer
    lbz     r3,  (moveTimer - mainLoop)(r14)
    cmpwi   r3,  0
    beq     .menuDoInput_notJustMoved
    subi    r3, r3, 1
    stb     r3,  (moveTimer - mainLoop)(r14)

    # are inputs neutral?
    or      r3,  r7,  r8 # stick X, Y
    andi.   r3,  r3,  0xF8 # deadzone
    or.     r3,  r3,  r6 # buttons too
    bne     .menuDoInput_endNoClose
    stb     r3,  (moveTimer - mainLoop)(r14) # inputs are neutral

.menuDoInput_notJustMoved:
    andi.   r4,  r6, PAD_BUTTON_B
    bne     .menuDoInput_close

    cmpwi   r7, -8
    ble     .menuDoInput_left
    cmpwi   r7,  8
    bge     .menuDoInput_right
    cmpwi   r8, -8
    ble     .menuDoInput_up
    cmpwi   r8,  8
    bge     .menuDoInput_down
    andi.   r4,  r6, PAD_BUTTON_A
    bne     .menuDoInput_use

.menuDoInput_endNoClose:
    li      r3, 1 # don't close
.menuDoInput_end:
    lwz     r29, SP_LR_SAVE(r1)
    mtlr    r29 # restore LR
    lmw     r4,  SP_GPR_SAVE(r1) # not r3
    addi    r1,  r1, STACK_SIZE # restore stack ptr
    blr

.menuDoInput_close:
    li      r3,  0
    b       .menuDoInput_end

.menuDoInput_left:
    li      r4, -1

.menuDoInput_storeCursorPos:
    lbz     r5,  (numItems - mainLoop)(r14)
    lbz     r3,  (cursorPos - mainLoop)(r14)
    add     r3,  r3,  r4
    extsb   r3,  r3
    cmpwi   r3,  0
    blt     .menuDoInput_wrapCursorToEnd
    cmpw    r3,  r5
    blt     .menuDoInput_noWrapCursor
    # cursor pos > # items
    li      r3,  0

.menuDoInput_noWrapCursor:
    stb     r3,  (cursorPos - mainLoop)(r14)
    li      r3,  5
    stb     r3,  (moveTimer - mainLoop)(r14)
    b       .menuDoInput_endNoClose

.menuDoInput_wrapCursorToEnd:
    addi    r3,  r5,  -1
    b       .menuDoInput_noWrapCursor

.menuDoInput_right:
    li      r4,  1
    b       .menuDoInput_storeCursorPos

.menuDoInput_up:
    li      r4, -8
    b       .menuDoInput_storeCursorPos

.menuDoInput_down:
    li      r4,  8
    b       .menuDoInput_storeCursorPos

.menuDoInput_use: # use selected item
    addi    r3,  r14, s_useItem - mainLoop
    lhz     r4,  (curItemBit - mainLoop)(r14)
    CALL    OSReport
    CALL    0x8004a868 # waitNextFrame

    lhz     r3,  (curItemBit - mainLoop)(r14)
    LOADWH  r4,  0x803dd8c2 # selectedItem
    STOREH  r3,  0x803dd8c2, r4 # itemBeingUsed
    STOREH  r3,  0x803dd8c0, r4 # cMenuSelectedItemBit
    li      r3,  0
    STOREB  r3,  0x803dd795, r4 # cMenuOpen
    li      r3,  1 # XXX item->fieldF
    STOREB  r3,  0x803dd8b8, r4 # isItemBeingUsed
    #li      r3,  0
    STOREW  r3,  0x803dd8a8, r4
    STOREB  r3,  0x803dd7d4, r4
    li      r3,  1
    STOREH  r3,  0x803dd8d6, r4
    LOADW   r3,  pPlayer
    lwz     r4,  0xB8(r3)
    li      r5,  -1
    sth     r5,  0x080A(r4)
    LOAD    r4,  0x00080001 # XXX depends on page
    li      r5,  0
    lhz     r6,  (curItemBit - mainLoop)(r14)
    CALL    0x800378c4 # use item
    #CALL    0x8004a868 # waitNextFrame
    #li      r3,  0 # pad
    #li      r4,  PAD_BUTTON_A|PAD_BUTTON_Y
    #CALL    0x80014b3c # buttonDisable

    b        .menuDoInput_close


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
    bl      menuDrawBoxOpaque # again to make it more opaque

    addi    r3,  r14, s_title - mainLoop
    li      r4,  0 # box ID
    li      r5,  0 # X
    li      r6,  0 # Y
    CALL    gameTextShowStr

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
    lhz     r3,  0x00(r19) # itemBit
    sth     r3,  (curItemBit - mainLoop)(r14)
    LOADWH  r4,  0x803dd8c2 # selectedItem
    STOREH  r3,  0x803dd8c0, r4 # cMenuSelectedItemBit


.drawMenu_notSelected:
    addi    r21, r21, 1 # next grid pos
.drawMenu_skipItem:
    addi    r17, r17, 1
    cmpwi   r17, 0x3D
    blt     .drawMenu_nextItem

.drawMenu_end:
    stb    r21, (numItems - mainLoop)(r14)
    lwz    r29, SP_LR_SAVE(r1)
    mtlr   r29 # restore LR
    lmw    r3,  SP_GPR_SAVE(r1)
    addi   r1,  r1, STACK_SIZE # restore stack ptr
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

curItemBit: .short 0 # GameBit for current item at cursor
isVisible:  .byte 0 # is inventory menu open?
cursorPos:  .byte 0 # selected slot
moveTimer:  .byte 0 # frame countdown to move again
openTimer:  .byte 0 # frame countdown to respond after opening
cMenuTimer: .byte 0 # frame countdown to restore C menu
numItems:   .byte 0 # number of items on menu
s_title:    .string "Inventory"
s_useItem:  .string "Use item 0x%04X"
s_openTimer:.string "Open timer %d"
s_cTimer:   .string "C timer %d"
s_mash:     .string "PRESSING A NOW"
.align 4
