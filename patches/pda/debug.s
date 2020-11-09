.ascii "debug   " # 8 byte file ID for debug

drawItem_debugText:
    addi    r4,  r14, (s_DebugText - mainLoop)
    blr

adjItem_debugText: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_PAGE_DEBUG_TEXT
.debugSetPage:
    cmpwi   r3,  0
    bnelr
    stb     r4,  (menuPage - mainLoop)(r14)
    li      r3,  0
    stb     r3,  (menuSelItem - mainLoop)(r14) # reset cursor
    blr

#######################################################################

drawItem_debugMap:
    addi    r4,  r14, (s_Map - mainLoop)
    blr

adjItem_debugMap: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_PAGE_DEBUG_MAP
    b       .debugSetPage

#######################################################################

drawItem_debugData:
    addi    r4,  r14, (s_Data - mainLoop)
    blr

adjItem_debugData: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_PAGE_DEBUG_DATA
    b       .debugSetPage

#######################################################################

drawItem_debugRender:
    addi    r4,  r14, (s_Render - mainLoop)
    blr

adjItem_debugRender: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_PAGE_DEBUG_RENDER
    b       .debugSetPage

#######################################################################

drawItem_debugCheat:
    addi    r4,  r14, (s_EditPlayerState - mainLoop)
    blr

adjItem_debugCheat: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_PAGE_DEBUG_CHEAT
    b       .debugSetPage

#######################################################################

drawItem_textTest:
    addi    r4,  r14, (s_TextTest - mainLoop)
    lhz     r5,  (textTestId - mainLoop)(r14)
    blr

adjItem_textTest:
    # r3 = amount to adjust by (0 = A button)
    # r9 = sound effect to play after calling this
    lha     r4,  (textTestId - mainLoop)(r14)
    add     r4,  r4,  r3
    sth     r4,  (textTestId - mainLoop)(r14)
    cmpwi   r3,  0
    bnelr
    lhz     r3,  (textTestId - mainLoop)(r14)
    li      r6,  1 # hide HUD and lock player
    mflr    r20
    CALL    0x8012ea5c # params r4, r5 are unused
    mtlr    r20
    blr

#######################################################################

drawItem_frameAdvance:
    addi    r4,  r14, (s_frameAdv - mainLoop)
    blr

adjItem_frameAdvance: # r3 = amount to adjust by (0=A button)
    li      r4,  0
    LOADWH  r5,  timeStop
    STOREB  r4,  timeStop, r5
    blr

#######################################################################
