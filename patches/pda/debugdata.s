.ascii "dbgdata " # 8 byte file ID for debug

drawItem_gameBits:
    addi    r4,  r14, (s_GameBits - mainLoop)
    blr

adjItem_gameBits: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_ID_GAMEBIT
    b       .debug_setMenu

#######################################################################

drawItem_saveGame:
    addi    r4,  r14, (s_SaveGame - mainLoop)
    blr

adjItem_saveGame: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_ID_SAVEGAME
    b       .debug_setMenu

#######################################################################

drawItem_heap:
    addi    r4,  r14, (s_Heap - mainLoop)
    blr

adjItem_heap: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_ID_HEAP_LIST
    b       .debug_setMenu

#######################################################################

drawItem_hexedit:
    addi    r4,  r14, (s_MemEdit - mainLoop)
    blr

adjItem_hexedit: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_ID_HEX_EDIT
    b       .debug_setMenu

#######################################################################
