drawItem_PDAHUD:
    addi    r4, r14, (s_PDAHUD - mainLoop)
    LOADWH  r7, pdaOn
    LOADBL2 r6, pdaOn, r7
    cmpwi   r6, 0
    beq     .draw_pda_off
    LOADBL2 r6, minimapMode, r7
    b       .draw_pda

.draw_pda_off:
    li      r6, 3

.draw_pda:
    slwi    r6, r6, 1 # r6 = offs into pdaModeStrs
    addi    r6, r6, .pdaModeStrs - mainLoop # r6 = addr of str offs
    lhzx    r6, r6, r14 # r6 = str offs from mainLoop
    add     r5, r6, r14 # r5 = str
    blr

.pdaModeStrs:
    .short s_map     - mainLoop
    .short s_compass - mainLoop
    .short s_info    - mainLoop
    .short s_off     - mainLoop


adjItem_PDAHUD:
    LOADWH  r7,  pdaOn
    LOADBL2 r6,  minimapMode, r7 # shares same upper half addr
    LOADBL2 r8,  pdaOn, r7
    cmpwi   r8,  0
    bne     .adjPDA
    li      r6,  3

.adjPDA:
    add     r6,  r6, r3 # r3 = adjust amount
    andi.   r6,  r6, 3
    cmpwi   r6,  3
    beq     .pdaOff
    STOREB  r6,  minimapMode, r7
    li      r8,  1
    b       .setPDA
.pdaOff:
    li      r8,  0

.setPDA:
    STOREB  r8,  pdaOn, r7
    stb     r8,  (menuWasPdaOn - mainLoop)(r14) # don't reset

    LOADWH  r5,  saveData
    LOADBL2 r7,  SAVEDATA_OPTIONS+saveData, r5
    andi.   r7,  r7,  (~SAVEDATA_OPTION_PDA_MODE) & 0xFFFF
    slwi    r6,  r6,  2
    or      r7,  r7,  r6
    STOREB  r7,  SAVEDATA_OPTIONS+saveData, r5
    blr
