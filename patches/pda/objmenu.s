.set OBJ_MENU_XPOS,   10
.set OBJ_MENU_YPOS,   20
.set OBJ_MENU_WIDTH, 300
.set OBJ_MENU_HEIGHT,448
.set OBJ_INFO_XPOS,OBJ_MENU_XPOS+OBJ_MENU_WIDTH+8
.set OBJ_INFO_YPOS,   20
.set OBJ_INFO_WIDTH, 300
.set OBJ_INFO_HEIGHT,448

objectMenu:
    # subroutine: runs the objects menu.
    # expects r14 = mainLoop.
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r0
    stw   r0, SP_LR_SAVE(r1)
    stmw  r13, SP_GPR_SAVE(r1)

    bl    menuHideHud

    lbz   r3, (objMenuState - mainLoop)(r14)
    slwi  r3, r3, 2
    addi  r4, r14, objMenu_Funcs - mainLoop
    lwzx  r3, r3, r4
    add   r3, r3, r14
    mtspr CTR, r3
    bctrl

    b       menuEndSub

objMenu_Funcs:
    .int objMenu_List - mainLoop


objMenu_List: # draw list of objects.
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r0
    stw   r0, SP_LR_SAVE(r1)
    stmw  r13, SP_GPR_SAVE(r1)

    bl      objMenu_List_doInput

    # get the camera in r21 and focused object in r22
    LOADW   r21, pCamera
    li      r22, 0
    cmpwi   r21, 0
    beq     .objMenu_List_noCamera
    lwz     r22, 0xA4(r21)

.objMenu_List_noCamera: # draw the list
    # draw the boxes
    li      r3, OBJ_MENU_XPOS   # X
    li      r4, OBJ_MENU_YPOS   # Y
    li      r5, OBJ_MENU_WIDTH  # width
    li      r6, OBJ_MENU_HEIGHT # height
    li      r20, 255 # opacity
    bl      menuDrawBox

    li      r3, OBJ_INFO_XPOS   # X
    li      r4, OBJ_INFO_YPOS   # Y
    li      r5, OBJ_INFO_WIDTH  # width
    li      r6, OBJ_INFO_HEIGHT # height
    bl      menuDrawBox

    # get the object ptrs
    lhz     r17, (objMenuIdx - mainLoop)(r14)
    LOADWH  r15, loadedObjects
    LOADWL2 r16, numLoadedObjs, r15
    LOADWL2 r15, loadedObjects, r15
    cmpwi   r16, 0
    beq     .objMenu_List_noObjs
    slwi    r4,  r17, 2
    lwzx    r18, r4, r15   # r18 = ObjInstance*
    mr      r3, r18

    bl      objMenu_drawCurObject
    # first item (selected) in blue
    LOAD    r3, 0x00FFFFFF
    bl      menuSetTextColor

    li      r20, OBJ_MENU_YPOS + 8 # string Y pos

.objMenu_List_nextObj:
    slwi    r4,  r17, 2
    lwzx    r18, r4, r15   # r18 = ObjInstance*
    cmpwi   r18, 0
    beqlr
    lwz     r19, 0x50(r18) # r19 = ObjectFileStruct*

    cmpw    r18, r22 # is this the focused object?
    bne     .objMenu_List_notFocused

    LOAD    r3, 0xFF00FFFF
    bl      menuSetTextColor

.objMenu_List_notFocused:
    # make line
    addi    r3,  r1, SP_STR_BUF
    addi    r4, r14, fmt_objListEntry - mainLoop
    mr      r5, r17 # idx
    #mr      r6, r18 # addr
    lwz     r6, 0x44(r18) # id and objdef
    CALL    sprintf

    # manually copy the name since it's not terminated
    # "0000 00000000 " = 14 chars
    addi    r3, r1, SP_STR_BUF + 14
    addi    r4, r19, 0x91 # name
    li      r5, 11
    CALL    strncpy

    addi    r3,  r1, SP_STR_BUF
    li      r4,  0
    stb     r4,  25(r3) # strncpy doesn't do this if src is long
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  OBJ_MENU_XPOS + 8  # X pos
    mr      r6,  r20 # Y pos
    CALL    gameTextShowStr

    LOAD    r3, 0xFFFFFFFF
    bl      menuSetTextColor

    # next line
    addi    r17, r17, 1
    cmpw    r17, r16 # obj = numLoadedObjs?
    bge     menuEndSub
    addi    r20, r20, LINE_HEIGHT
    cmpwi   r20, OBJ_MENU_YPOS + OBJ_MENU_HEIGHT - LINE_HEIGHT
    blt     .objMenu_List_nextObj

    b       menuEndSub


.objMenu_List_noObjs:
    addi    r3, r14, s_noObjs - mainLoop
    li      r5, OBJ_MENU_XPOS
    li      r6, OBJ_MENU_XPOS
    bl      menuDrawStr2
    b       menuEndSub


objMenu_List_doInput:
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r0
    stw   r0, SP_LR_SAVE(r1)
    stmw  r13, SP_GPR_SAVE(r1)

    lhz     r17, (objMenuIdx - mainLoop)(r14)
    LOADWH  r15, loadedObjects
    LOADWL2 r16, numLoadedObjs, r15
    LOADWL2 r15, loadedObjects, r15
    slwi    r4,  r17, 2
    lwzx    r18, r4, r15   # r18 = ObjInstance*

    # check the buttons
    bl      menuGetInput
    # r3=buttons, r4=stick X, r5=stick Y,
    # r6=CX, r7=CY, r8=L, r9=R
    andi.   r10, r3, PAD_BUTTON_B
    bne     .objMenu_close
    andi.   r10, r3, PAD_BUTTON_Z
    bne     .objMenu_focus
    andi.   r10, r3, PAD_BUTTON_X
    bne     .objMenu_delete
    andi.   r10, r3, PAD_BUTTON_Y
    bne     .objMenu_goto
    andi.   r10, r3, PAD_BUTTON_MENU
    bne     .objMenu_toPlayer

    # check analog stick
    cmpwi   r5, 0x10
    bgt     .objMenu_List_up
    cmpwi   r5, -0x10
    blt     .objMenu_List_down

    # check C stick - same as analog but no delay
    cmpwi   r7, 0x10
    bgt     .objMenu_List_up
    cmpwi   r7, -0x10
    blt     .objMenu_List_down

    # check L/R - jump by page
    cmpwi   r8, 0x04
    bgt     .objMenu_List_prevPage
    cmpwi   r9, 0x04
    bgt     .objMenu_List_nextPage

    b       menuEndSub

.objMenu_List_up: # up pressed
    subi    r17, r17, 1
.objMenu_List_up2:
    cmpwi   r17, 0
    bge     .objMenu_List_storeIdx
    addi    r17, r16, -2
    # by adding -2 there and then falling into the Down
    # code, which adds 1, we avoid a branch.

.objMenu_List_down: # down pressed
    addi    r17, r17, 1
.objMenu_List_down2:
    cmpw    r17, r16
    blt     .objMenu_List_storeIdx
    li      r17, 0

.objMenu_List_storeIdx:
    cmpwi   r17, 0
    blt     menuEndSub
    sth     r17, (objMenuIdx - mainLoop)(r14)
    # use a smaller repeat delay here because
    # this menu is long, and slow to render.
    # don't use any delay if using C stick.
    andi.   r7, r7, 0xF0 # deadzone
    bne     menuEndSub
    li      r3,  3
    stb     r3,  (menuJustMoved - mainLoop)(r14)
    b       menuEndSub

.objMenu_List_prevPage:
    subi    r17, r17, 0x40
    b       .objMenu_List_up2

.objMenu_List_nextPage:
    addi    r17, r17, 0x40
    b       .objMenu_List_down2

.objMenu_close:
    li      r3, 0
    sth     r3, (objMenuIdx - mainLoop)(r14)
    bl      returnToMainMenu
    b       menuEndSub

.objMenu_focus: # set the camera focus to the selected object
    # get the camera in r21
    LOADW   r21, pCamera
    #li      r22, 0
    cmpwi   r21, 0
    beq     .objMenu_focus_cantuse
    lwz     r3, 0xB8(r18)
    cmpwi   r3, 0 # has an AnimState*?
    beq     .objMenu_focus_cantuse
    # focusing the camera on an object with NULL AnimState*
    # will crash the game, so don't do that.
    stw     r18, 0xA4(r21)
    # since time is stopped we must manually update
    # the camera position.
    CALL    0x801030c0
    b       .objMenu_end

.objMenu_focus_cantuse:
    li      r3, 0
    li      r4, 0xFD # "can't use that item"
    CALL    audioPlaySound
    b       menuEndSub

.objMenu_delete: # delete the selected object
    mr      r3, r18
    CALL    objFree
.objMenu_end:
    li      r3,  MOVE_DELAY
    stb     r3,  (menuJustMoved - mainLoop)(r14)
    b       menuEndSub

.objMenu_goto: # move player to the selected object
    LOADW   r21, pPlayer
    cmpwi   r21, 0
    beq     .objMenu_end
    lwz     r3,  0x0C(r18)
    stw     r3,  0x0C(r21)
    stw     r3,  0x18(r21) # try to override hit detection...
    lwz     r3,  0x10(r18)
    stw     r3,  0x10(r21)
    stw     r3,  0x1C(r21)
    lwz     r3,  0x14(r18)
    stw     r3,  0x14(r21)
    stw     r3,  0x20(r21)
    b       .objMenu_end

.objMenu_toPlayer: # move cursor to Player object
    LOADW   r21, pPlayer
    LOADWH  r15, loadedObjects
    LOADWL2 r16, numLoadedObjs, r15
    LOADWL2 r15, loadedObjects, r15
.objMenu_toPlayer_next:
    subi    r16, r16, 1
    slwi    r4,  r16, 2
    lwzx    r3,  r15, r4
    cmpw    r3,  r21
    beq     .objMenu_toPlayer_found

    cmpwi   r16, 0
    bne     .objMenu_toPlayer_next
    b       .objMenu_end

.objMenu_toPlayer_found:
    sth     r16, (objMenuIdx - mainLoop)(r14)
    b       .objMenu_end


objMenu_drawCurObject:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r0
    stw     r0, SP_LR_SAVE(r1)
    stmw    r13, SP_GPR_SAVE(r1)

    mr      r18, r3
    LOAD    r3, 0xFFFFFFFF
    bl      menuSetTextColor
    li      r19, OBJ_INFO_XPOS + 8
    li      r20, OBJ_INFO_YPOS + 8

    # address, coords
    lfs     f1, 0x0C(r18)
    fctiwz  f2,f1
    stfd    f2,SP_FLOAT_TMP(r1)
    lwz     r6,SP_FLOAT_TMP+4(r1)

    lfs     f1, 0x10(r18)
    fctiwz  f2,f1
    stfd    f2,SP_FLOAT_TMP(r1)
    lwz     r7,SP_FLOAT_TMP+4(r1)

    lfs     f1, 0x14(r18)
    fctiwz  f2,f1
    stfd    f2,SP_FLOAT_TMP(r1)
    lwz     r8,SP_FLOAT_TMP+4(r1)

    mr      r5, r18 # address
    addi    r4, r14, fmt_objListCoords - mainLoop
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    # ID, original coords
    addi    r4, r14, fmt_objListNoSeq - mainLoop
    lwz     r3, 0x4C(r18) # seq
    cmpwi   r3, 0
    beq     .objMenu_drawCurObject_noSeq
    addi    r4, r14, fmt_objListOrigPos - mainLoop
    lwz     r5, 0x14(r3) # ID

    lfs     f1, 0x08(r3) # orig X
    fctiwz  f2,f1
    stfd    f2,SP_FLOAT_TMP(r1)
    lwz     r6,SP_FLOAT_TMP+4(r1)

    lfs     f1, 0x0C(r3) # orig Y
    fctiwz  f2,f1
    stfd    f2,SP_FLOAT_TMP(r1)
    lwz     r7,SP_FLOAT_TMP+4(r1)

    lfs     f1, 0x10(r3) # orig Z
    fctiwz  f2,f1
    stfd    f2,SP_FLOAT_TMP(r1)
    lwz     r8,SP_FLOAT_TMP+4(r1)

.objMenu_drawCurObject_noSeq:
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    # ObjectFileStruct
    addi    r4, r14, fmt_objListFile - mainLoop
    lwz     r5, 0x50(r18) # files
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    # flags
    addi    r4, r14, fmt_objListFlags - mainLoop
    lhz     r5, 0x06(r18) # flags
    lbz     r6, 0xAF(r18) # flags
    lbz     r7, 0xB0(r18) # flags
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    # slot, map
    addi    r4, r14, fmt_objListMap - mainLoop
    lbz     r5, 0xAE(r18) # slot
    lbz     r6, 0x34(r18) # map
    lbz     r7, 0xAC(r18) # map 2
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    # seq
    addi    r4, r14, fmt_objListSeq - mainLoop
    lwz     r5, 0x4C(r18) # seq
    lhz     r6, 0xB4(r18) # curSeq
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    # event
    addi    r4, r14, fmt_objListEvent - mainLoop
    lwz     r5, 0x60(r18) # event
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    # models
    li      r7, 0
    lis     r8, 0x81FF
    lis     r9, 0x8000
    addi    r4, r14, fmt_objListModel - mainLoop
    lwz     r5, 0x7C(r18) # models
    lbz     r6, 0xAD(r18) # curModel
    cmpwi   r6, 0xFF
    beq     .objMenu_drawCurObject_noModel
    slwi    r7, r6, 4
    lwzx    r7, r5, r7 # get model

    cmpw    r7, r8 # sanity check pointer
    bgt     .objMenu_drawCurObject_noModel
    cmpw    r7, r9
    blt     .objMenu_drawCurObject_noModel
    lwz     r7, 0(r7) # get ModelFileHeader

    cmpw    r7, r8 # sanity check pointer
    bgt     .objMenu_drawCurObject_noModel
    cmpw    r7, r9
    blt     .objMenu_drawCurObject_noModel
    lhz     r7, 4(r7) # get model ID

.objMenu_drawCurObject_noModel:
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    # line 7: state
    addi    r4, r14, fmt_objListState - mainLoop
    lwz     r5, 0xB8(r18) # state
    bl      menuPrintf
    addi    r20, r20, LINE_HEIGHT

    # instructions
    addi    r3,  r14, fmt_objListInstrs - mainLoop
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  OBJ_INFO_XPOS + 8 # X pos
    li      r6,  OBJ_INFO_YPOS + OBJ_INFO_HEIGHT - LINE_HEIGHT # Y pos
    bl      menuDrawStr

    # this doesn't work. we need to move the object in front of the
    # camera, not to it. but we need to figure out the angle and distance.
    # or somehow change the modelview matrix.
    # maybe gxSetProjection(&hudMatrix,1);
.if 0
    # back up object coords
    lwz     r3, 0x0C(r18)
    stw     r3, SP_STR_BUF(r1)
    lwz     r3, 0x10(r18)
    stw     r3, (SP_STR_BUF+4)(r1)
    lwz     r3, 0x14(r18)
    stw     r3, (SP_STR_BUF+8)(r1)

    lwz     r3, 0x0C(r21) # camera coords
    stw     r3, 0x0C(r18)
    lwz     r3, 0x10(r21)
    stw     r3, 0x10(r18)
    lwz     r3, 0x14(r21)
    stw     r3, 0x14(r18)

    mr      r3, r18
    CALL    0x80041ac4 # ObjRenderModel

    lwz     r3, SP_STR_BUF(r1)
    stw     r3, 0x0C(r18)
    lwz     r3, (SP_STR_BUF+4)(r1)
    stw     r3, 0x10(r18)
    lwz     r3, (SP_STR_BUF+8)(r1)
    stw     r3, 0x14(r18)
.endif

    b       menuEndSub


objMenuIdx: .short 0 # which object
objMenuState: .byte 0 # 0=list 1=view
.byte 0 # padding
