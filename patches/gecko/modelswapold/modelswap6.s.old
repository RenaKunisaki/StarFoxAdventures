.text
.include "common.s"

# Patch ANIM.tab loading; set Krystal anim offsets we loaded in patch 5
GECKO_BEGIN_PATCH 0x80045ad8 # lwz r3, -0x6554(r13)
# just before a call to stackPush
# r3, r4 are free
# nearly identical to patch 2 and 4
b start

.set MODANIM_BIN,0x8035f4a0
.set ANIM_TAB,0x8035f4a4
.set NUM_ANIMS,0x35D
.set MODEL_ID,0x4e8
.set MODANIM_OFFSET,0x1E60

# stack offsets
.set STACK_SIZE,0x100 # how much to reserve
.set SP_LR_SAVE,0x10
.set SP_R0_SAVE,0x14
.set SP_R11_SAVE,0x18
.set SP_R13_SAVE,0x1C
.set SP_GPR_SAVE,0x80

filePath:
    .string "animtest/ANIM.TAB"

# using .align generates unnecessary extra padding.
.byte 0, 0
start:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r3
    stw     r3,  SP_LR_SAVE(r1)
    stw     r0,  SP_R0_SAVE(r1)
    stw     r11, SP_R11_SAVE(r1)
    stw     r13, SP_R13_SAVE(r1)
    addi    r11, r1, SP_GPR_SAVE
    CALL    __save_gpr

    # get file path in r3
    mflr r7
    bl   .getpc
    .getpc:
        mflr r3
        mtlr r7 # restore LR
        addi r3, r3, (filePath - .getpc)@l

    # load "warlock/ANIM.tab"
    li      r4, 0 # we don't need size
    CALL    loadFileByPath
    # now r3 = data
    #stw     r3, SP_FILE_BUFFER(r1)

    # get offsets of already loaded files
    lis     r9, MODANIM_BIN@h
    ori     r9, r9, MODANIM_BIN@l
    lwz     r4, 0(r9) # r4 = MODANIM.BIN
    lwz     r5, 4(r9) # r5 = ANIM.TAB
    li      r6, 0
    addi    r4, r4, MODANIM_OFFSET

    # get the anim offset, stored by previous code.
    lis     r8, 0x8180
    lwz     r8, -0x0C(r8)

    .nextAnimId:
        lhzx    r0, r4, r6 # get anim ID
        andi.   r9, r0, 0x8000
        bne     .skip

        # get the entry for this anim ID in ANIM.TAB
        slwi    r9, r0, 2   # ID -> offset
        lwzx    r7, r9, r3  # get the entry
        add     r7, r7, r8  # add the offset we loaded at
        stwx    r7, r9, r5  # store the entry

    .skip:
        addi    r6, r6, 2
        cmpwi   r6, NUM_ANIMS * 2
        bne     .nextAnimId

    # now all entries in the loaded ANIM.TAB should match the ones
    # in warlock/ANIM.TAB plus the offset that we appended
    # the warlock/ANIM.BIN at.

end:
    # free the file buffer
    #lwz     r3, SP_FILE_BUFFER(r1)
    CALL    free
    addi    r11, r1, SP_GPR_SAVE
    CALL    __restore_gpr

    lwz     r13, SP_R13_SAVE(r1)
    lwz     r11, SP_R11_SAVE(r1)
    lwz     r0,  SP_R0_SAVE(r1)
    lwz     r3,  SP_LR_SAVE(r1) # restore LR
    mtlr    r3
    addi    r1,  r1, STACK_SIZE # restore stack ptr
    lwz     r3,  -0x6554(r13) # replaced

GECKO_END_PATCH
