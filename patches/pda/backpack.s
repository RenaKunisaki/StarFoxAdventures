.ascii "backpack" # 8 byte file ID for debug

drawItem_backpack:
    addi    r4,  r14, (s_backpack - mainLoop)

    LOADWH  r5,  saveData
    LOADBL2 r7,  SAVEDATA_OPTIONS+saveData, r5
    andi.   r7,  r7,  SAVEDATA_OPTION_BACKPACK
    srwi    r7,  r7,  4
    addi    r5,  r14, backpackStrPtrs - mainLoop
    lhzx    r5,  r5,  r7
    add     r5,  r5,  r14
    blr

adjItem_backpack: # r3 = amount to adjust by (0=A button)
    cmpwi   r3,  0
    bne     .adjItem_backpack_notA
    li      r3,  1
.adjItem_backpack_notA:
    LOADWH  r5,  saveData
    LOADBL2 r7,  SAVEDATA_OPTIONS+saveData, r5
    andi.   r6,  r7,  SAVEDATA_OPTION_BACKPACK
    srwi    r8,  r6,  5
    add     r8,  r8,  r3
    andi.   r8,  r8,  0x03
    cmpwi   r8,  3
    bne     .adjItem_backpack_not3
    add     r8,  r8,  r3
    andi.   r8,  r8,  0x03
.adjItem_backpack_not3:
    slwi    r8,  r8,  5
    andi.   r7,  r7,  (~SAVEDATA_OPTION_BACKPACK) & 0xFFFF
    or      r7,  r7,  r8
    STOREB  r7,  SAVEDATA_OPTIONS+saveData, r5
    blr


backpackStrPtrs:
    .short s_Normal - mainLoop
    .short s_on     - mainLoop
    .short s_off    - mainLoop
    .short 0 # padding

#######################################################################
