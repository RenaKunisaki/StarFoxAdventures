drawItem_furEffect:
    addi    r4,  r14, (s_FurEffect - mainLoop)
    addi    r5,  r14, (s_Normal - mainLoop)
    LOADB   r6,  0x800414E3
    cmpwi   r6,  0
    beq     .drawFurEffect_off
    addi    r5,  r14, (s_Always - mainLoop)

.drawFurEffect_off:
    blr

adjItem_furEffect: # r3 = amount to adjust by (0=A button)
#    addi    r3,  r14,  (.furFx_addrs  - mainLoop) - 4
#    addi    r4,  r14,  (.furFx_values - mainLoop) - 4
#    li      r5,  1     # number of values
#.adjItem_furEffect_next:
#    lwzu    r6,  4(r3) # get addr
#    lwzu    r7,  4(r4) # get XOR
#    lwz     r8,  0(r6) # get current value
#    xor     r8,  r8,  r7
#    stw     r8,  0(r6)
    LOAD    r6,  0x800414E0
    lwz     r7,  0(r6)
    xori    r7,  r7,  0x1312
    stw     r7,  0(r6)
    li      r8,  0
    icbi    r8,  r6 # flush instruction cache
#    subi    r5,  r5,  1
#    cmpwi   r5,  0
#    bne     .adjItem_furEffect_next
    blr

#.furFx_addrs:
    #.int 0x800414E4
#    .int 0x800414E0
    #.int 0x80041518
    #.int 0x800415D0
    #.int 0x80041640
    #.int 0x8004164C
#.furFx_values: # original XOR replacement
    #.int 0x40820028 ^ 0x48000028
#    .int 0x2C000000 ^ 0x2C001312
    # are any of these necessary?
    #.int 0x41820020 ^ 0x48000020
    #.int 0x4082003C ^ 0x4800003C
    #.int 0x40810090 ^ 0x60000000
    #.int 0x40810008 ^ 0x60000000

# we could also add "never" by putting a blr at 800414b4

#######################################################################
