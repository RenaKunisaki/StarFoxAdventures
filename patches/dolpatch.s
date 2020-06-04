# patches main.dol file to load debug code as needed
.text
.include "common.s"

# patch at 802408a8: 4B FF FB 59  bl ClearArena (in _osInit())
# or better, 80021038: 48 11 6C F1  bl installBsodHandlers (in init())
# can probably insert at 80003E6C - 80004338, room for 307 instructions
# 80003100 in RAM => 0x100 in DOL file
# but that's only text0
# 80021038 is text1, which should start at 0x2620, and 800066E0
# the proper offset is 1cf78
# we should make a script for this. it's really simple.
# the DOL file just starts with 18 offsets, then 18 addresses, then 18 lengths.
# each is one section, with text being the first 7 and data being the rest.
# then BSS addr, BSS len, entry point.
# we could potentially even just add a section.
# but for now I manually patched this into the DOL and it works.

# for now I'm gonna be lazy and just hardcode the jump
# eventually a script should use these ints to know where to patch.
.int 0x80021038, 0x4BFE2E35
.int 0x80003E6C

constants:
    .set STACK_SIZE,0x40 # how much to reserve
    .set SP_LR_SAVE,0x10
    .set SP_R0_SAVE,0x14


start:
    stwu r1, -STACK_SIZE(r1) # get some stack space
    mflr r3
    stw  r3,  SP_LR_SAVE(r1)
    stw  r0,  SP_R0_SAVE(r1)
    CALL 0x80137d28 # installBsodHandlers()
    lwz  r0,  SP_R0_SAVE(r1)

    # get filePath address
    bl .getpc
    .getpc:
        mflr r3
        addi r3, r3, (filePath - .getpc)@l

    # load the file
    li      r4, 0 # we don't need size
    CALL    loadFileByPath
    cmpwi   r3, 0
    beq     .done # file doesn't exist

    # we loaded the file, so jump to it.
    # the file's code can free the buffer if it wants when done.
    mtspr CTR, r3
    bctrl

.done:
    lwz  r3, SP_LR_SAVE(r1)
    mtlr r3 # restore LR
    addi r1, r1, STACK_SIZE # restore stack ptr
    blr


filePath:
    .string "debug.bin"
