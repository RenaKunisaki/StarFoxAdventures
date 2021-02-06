.ascii "dbgcheat" # 8 byte file ID for debug

debugCheatAdjustValue:
    # r5 = value, r6 = max, r20 = adjust amount
    # if adjusting fast, clamps to zero/max.
    # if pressing A, resets to max.
    # if adjust is 0 (A button), refill to max.
    cmpwi   r20, 0
    bne     .debugCheatAdjustValue_noRefill
    mr      r5,  r6 # refill to max

.debugCheatAdjustValue_noRefill:
    # if adjust is by more than 1, clamp to zero/max.
    add     r5,  r5,  r20
    cmpwi   r20, -1
    blt     .debugCheatAdjustValue_clamp
    cmpwi   r20,  1
    blelr

.debugCheatAdjustValue_clamp:
    cmpwi   r5,  0
    blt     .debugCheatAdjustValue_clampToZero # cur < 0 ?
    cmpw    r5,  r6
    bltlr   # cur < max ?
    mr      r5,  r6 # cur = max
    blr

.debugCheatAdjustValue_clampToZero:
    li      r5,  0
    blr


drawItem_curHP:
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r20, r3
    CALL    SaveGame_getCurCharacterState
    lbz     r5,  0x00(r3)
    addi    r4,  r14, (s_curHP - mainLoop)

dbgCheatDrawEnd:
    mr      r3,  r20
    b       menuEndSub

adjItem_curHP: # r3 = amount to adjust by (0=A button)
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r20, r3
    mr      r21, r9 # sound effect ID
    CALL    SaveGame_getCurCharacterState
    lbz     r5,  0x00(r3) # get cur
    lbz     r6,  0x01(r3) # get max
    bl      debugCheatAdjustValue
    stb     r5,  0x00(r3)

dbgCheatAdjEnd:
    mr      r9,  r21
    b       menuEndSub

#######################################################################

drawItem_maxHP:
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r20, r3
    CALL    SaveGame_getCurCharacterState
    lbz     r5,  0x01(r3)
    addi    r4,  r14, (s_maxHP - mainLoop)
    b       dbgCheatDrawEnd

adjItem_maxHP: # r3 = amount to adjust by (0=A button)
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r20, r3
    mr      r21, r9 # sound effect ID
    CALL    SaveGame_getCurCharacterState
    lbz     r5,  0x01(r3)
    add     r5,  r5,  r20
    stb     r5,  0x01(r3)

    b       dbgCheatAdjEnd

#######################################################################

drawItem_curMP:
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r20, r3
    CALL    SaveGame_getCurCharacterState
    lha     r5,  0x04(r3)
    addi    r4,  r14, (s_curMP - mainLoop)
    b       dbgCheatDrawEnd

adjItem_curMP: # r3 = amount to adjust by (0=A button)
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r20, r3
    mr      r21, r9 # sound effect ID
    CALL    SaveGame_getCurCharacterState
    lha     r5,  0x04(r3) # get cur
    lha     r6,  0x06(r3) # get max
    bl      debugCheatAdjustValue
    sth     r5,  0x04(r3)
    b       dbgCheatAdjEnd

#######################################################################

drawItem_maxMP:
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r20, r3
    CALL    SaveGame_getCurCharacterState
    lha     r5,  0x06(r3)
    addi    r4,  r14, (s_maxMP - mainLoop)
    b       dbgCheatDrawEnd

adjItem_maxMP: # r3 = amount to adjust by (0=A button)
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r20, r3
    mr      r21, r9 # sound effect ID
    CALL    SaveGame_getCurCharacterState
    lha     r5,  0x06(r3)
    add     r5,  r5,  r20
    sth     r5,  0x06(r3)

    b       dbgCheatAdjEnd

#######################################################################

drawItem_curMoney:
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r20, r3
    CALL    SaveGame_getCurCharacterState
    lbz     r5,  0x08(r3)
    addi    r4,  r14, (s_curMoney - mainLoop)
    b       dbgCheatDrawEnd

adjItem_curMoney: # r3 = amount to adjust by (0=A button)
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r20, r3
    mr      r21, r9 # sound effect ID
    CALL    SaveGame_getCurCharacterState
    lbz     r5,  0x08(r3)
    li      r6,  255
    bl      debugCheatAdjustValue
    stb     r5,  0x08(r3)
    b       dbgCheatAdjEnd

#######################################################################

drawItem_curLives:
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r20, r3
    CALL    SaveGame_getCurCharacterState
    lbz     r5,  0x09(r3)
    addi    r4,  r14, (s_curLives - mainLoop)
    b       dbgCheatDrawEnd

adjItem_curLives: # r3 = amount to adjust by (0=A button)
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r20, r3
    mr      r21, r9 # sound effect ID
    CALL    SaveGame_getCurCharacterState
    lbz     r5,  0x09(r3) # get cur
    lbz     r6,  0x0A(r3) # get max
    bl      debugCheatAdjustValue
    stb     r5,  0x09(r3)
    b       dbgCheatAdjEnd

#######################################################################

drawItem_maxLives:
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r20, r3
    CALL    SaveGame_getCurCharacterState
    lbz     r5,  0x0A(r3)
    addi    r4,  r14, (s_maxLives - mainLoop)
    b       dbgCheatDrawEnd

adjItem_maxLives: # r3 = amount to adjust by (0=A button)
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r20, r3
    mr      r21, r9 # sound effect ID
    CALL    SaveGame_getCurCharacterState
    lbz     r5,  0x0A(r3)
    add     r5,  r5,  r20
    stb     r5,  0x0A(r3)

    b       dbgCheatAdjEnd

#######################################################################

drawItem_unlockAll:
    addi    r4,  r14, (s_unlockAll - mainLoop)
    blr

adjItem_unlockAll: # r3 = amount to adjust by (0=A button)
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)
    mr      r21, r9

    cmpwi   r3,  0
    bne     dbgCheatAdjEnd

    addi    r20, r14, (.unlockAll_table - mainLoop) - 2
.unlockAll_next:
    lhau    r3,  2(r20)
    cmpwi   r3,  0
    blt     .unlockAll_done
    li      r4,  0x7FFF
    CALL    mainSetBits
    b       .unlockAll_next

.unlockAll_done:
    b       dbgCheatAdjEnd

#######################################################################

drawItem_killMe:
    addi    r4,  r14, (s_killMe - mainLoop)
    blr

adjItem_killMe: # r3 = amount to adjust by (0=A button)
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0,  SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)
    mr      r21, r9

    cmpwi   r3,  0
    bne     dbgCheatAdjEnd

    LOADW   r3,  pPlayer
    CALL    playerDie
    b       dbgCheatAdjEnd

.unlockAll_table: # GameBits to set to max
    .short 0x0025 # HaveTrickysBall
    .short 0x002D # HaveFireballSpell
    .short 0x0040 # HaveSharpClawDisguise
    .short 0x0075 # HaveStaff
    .short 0x00C1 # NumTrickyFoods
    .short 0x00DD # HaveCallTricky
    .short 0x0107 # HaveGroundQuake
    .short 0x013D # NumFireflies
    .short 0x013E # HaveFireflyLantern
    .short 0x0245 # HaveTrickyFlame
    .short 0x03F5 # NumFuelCells
    .short 0x03F9 # WorldMapDarkIce
    .short 0x03FA # WorldMapCloudFort
    .short 0x03FB # WorldMapWallCity
    .short 0x03FC # WorldMapDragRock
    .short 0x04E4 # CanUseTricky
    .short 0x059D # HaveVolcanoMap
    .short 0x059E # HaveDarkIceMap
    .short 0x05A0 # HaveSnowWastesMap
    .short 0x05A1 # HaveCloudFortMap
    .short 0x05A2 # HaveLightFootMap
    .short 0x05A3 # HaveHollowMap
    .short 0x05BD # HaveOpenPortal
    .short 0x05CE # HaveIceBlast
    .short 0x05D6 # NumFirefliesNotShown
    .short 0x066C # NumBombSpores
    .short 0x07DD # HaveDragRockMap
    .short 0x07E5 # HaveKrazoaMap
    .short 0x07E9 # HaveOceanMap
    .short 0x082E # HaveWallCityMap
    .short 0x082F # HaveCapeClawMap
    .short 0x0835 # HaveMoonPassMap
    .short 0x086A # NumMoonSeeds
    .short 0x090D # DidCollectMagic
    .short 0x090E # DidCollectBigHealth
    .short 0x090F # DidCollectApple
    .short 0x0912 # DidSeeWarpPad
    .short 0x0919 # Have50ScarabBag
    .short 0x091A # Have100ScarabBag
    .short 0x091B # Have200ScarabBag
    .short 0x0957 # HaveStaffBooster
    .short 0x0958 # HaveLaserSpell
    .short 0x0C55 # HaveSuperQuake
    .short 0x0C64 # HaveViewfinder
    .short 0x0CC0 # DidCollectBafomdad
    .short 0x0EB1 # HaveMagic
    .short 0x0EB2 # HaveBafomdadHolder
    .short 0xFFFF # end
