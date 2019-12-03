.text
.include "common.s"

# patch mergeTableFiles() case MODELS.tab
# insert the offset we loaded Krystal at into the table
GECKO_BEGIN_PATCH 0x80043d78 # li r9, 0x0800

.set MODELS_TAB_MAIN,0x8035f490
.set MODELS_TAB_SUB,0x8035f4fc
.set KRYSTAL_MODEL_ID,0x4E8
.set STACK_SIZE,0x100 # how much to reserve

start:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stmw    r3, 0x10(r1)

    # get the ptr to model data, stored by previous code.
    lis     r4, 0x8180
    lwz     r4, -4(r4)
    cmpwi   r4, 0
    beq     end # we didn't load the model
    # better to let it be a cube than to crash the game

    lis     r5, 0x8000 # set flags: compressed, use main map MODELS.bin
    or      r6, r4, r5

    # get main MODELS.TAB
    LOADWH  r7, MODELS_TAB_MAIN
    LOADWL  r3, MODELS_TAB_MAIN, r7
    cmpwi   r3, 0
    beq     .next

.doit:
    lwz     r4, (KRYSTAL_MODEL_ID * 4)(r3) # r4 = offset for Krystal model
    rlwinm. r5, r4, 4, 0xF # r5 = (r4 >> 28) & 0xF (the flag bits)
    cmpwi   r5, 0 # already loaded?
    bne     .next # then don't change it.

    # store it into map's MODELS.TAB
    stw     r6, (KRYSTAL_MODEL_ID * 4)(r3)

# try other slot
.next:
    LOADWL  r3, MODELS_TAB_SUB, r7
    cmpwi   r3, 0
    beq     end

    lwz     r4, (KRYSTAL_MODEL_ID * 4)(r3) # r4 = offset for Krystal model
    rlwinm. r5, r4, 4, 0xF # r5 = (r4 >> 28) & 0xF (the flag bits)
    cmpwi   r5, 0 # already loaded?
    bne     end # don't change it
    stw     r6, (KRYSTAL_MODEL_ID * 4)(r3)

end:
    lmw     r3, 0x10(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    li      r9, 0x0800 # replaced

GECKO_END_PATCH
