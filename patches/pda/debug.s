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

drawItem_debugMisc:
    addi    r4,  r14, (s_Misc - mainLoop)
    blr

adjItem_debugMisc: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_PAGE_DEBUG_MISC
    b       .debugSetPage

#######################################################################

drawItem_debugCamera:
    addi    r4,  r14, (s_Camera - mainLoop)
    blr

adjItem_debugCamera: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_PAGE_DEBUG_CAMERA
    b       .debugSetPage

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
