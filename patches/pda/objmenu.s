.set OBJ_MENU_XPOS,20
.set OBJ_MENU_YPOS,20
.set OBJ_MENU_WIDTH,600
.set OBJ_MENU_HEIGHT,400

objectMenu:
    # subroutine: runs the objects menu.
    # expects r14 = mainLoop.
    stwu  r1, -STACK_SIZE(r1) # get some stack space
    mflr  r0
    stw   r0, SP_LR_SAVE(r1)
    stmw  r13, SP_GPR_SAVE(r1)

    li     r3, 1
    LOADWH r4, hudHidden
    STOREB r3, hudHidden, r4

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
    # XXX hide the HUD when we're in the object list, as it
    # likes to render overtop of our menu.

    # draw the box
    li      r3, OBJ_MENU_XPOS   # X
    li      r4, OBJ_MENU_YPOS   # Y
    li      r5, OBJ_MENU_WIDTH  # width
    li      r6, OBJ_MENU_HEIGHT # height
    bl      menuDrawBox

    # first item (selected) in blue
    li      r3, 0
    li      r4, 255
    li      r5, 255
    li      r6, 255
    CALL    gameTextSetColor

    # get the object ptrs
    lhz     r17, (objMenuIdx - mainLoop)(r14)
    LOADWH  r15, loadedObjects
    LOADWL2 r16, numLoadedObjs, r15
    LOADWL2 r15, loadedObjects, r15
    slwi    r4,  r17, 2
    lwzx    r18, r4, r15   # r18 = ObjInstance*

    # draw the coords
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
    addi    r3,  r1, SP_STR_BUF
    CALL    sprintf

    addi    r3,  r1, SP_STR_BUF
    li      r4,  MENU_TEXTBOX_ID # box type
    li      r5,  OBJ_MENU_XPOS # X pos
    li      r6,  OBJ_MENU_YPOS + OBJ_MENU_HEIGHT + 16 # Y pos
    CALL    gameTextShowStr

    li      r20, OBJ_MENU_YPOS # string Y pos

.objMenu_List_nextObj:
    slwi    r4,  r17, 2
    lwzx    r18, r4, r15   # r18 = ObjInstance*
    cmpwi   r18, 0
    beqlr
    lwz     r19, 0x50(r18) # r19 = ObjectFileStruct*

    cmpw    r18, r22 # is this the focused object?
    bne     .objMenu_List_notFocused

    li      r3, 255
    li      r4, 0
    li      r5, 255
    li      r6, 255
    CALL    gameTextSetColor

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
    li      r5,  10  # X pos
    mr      r6,  r20 # Y pos
    CALL    gameTextShowStr

    li      r3, 255
    li      r4, 255
    li      r5, 255
    li      r6, 255
    CALL    gameTextSetColor

    # next line
    addi    r17, r17, 1
    cmpw    r17, r16 # obj = numLoadedObjs?
    bge     menuEndSub
    addi    r20, r20, LINE_HEIGHT
    cmpwi   r20, OBJ_MENU_YPOS + OBJ_MENU_HEIGHT
    blt     .objMenu_List_nextObj

    b       menuEndSub


objMenu_List_doInput: # return r3 = 0 to close, 1 to keep open
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

    b       menuEndSub

.objMenu_List_up: # up pressed
    subi    r17, r17, 1
    cmpwi   r17, 0
    bge     .objMenu_List_storeIdx
    addi    r17, r16, -2
    # by adding -2 there and then falling into the Down
    # code, which adds 1, we avoid a branch.

.objMenu_List_down: # down pressed
    addi    r17, r17, 1
    cmpw    r17, r16
    blt     .objMenu_List_storeIdx
    li      r17, 0

.objMenu_List_storeIdx:
    sth     r17, (objMenuIdx - mainLoop)(r14)
    # use a smaller repeat delay here because
    # this menu is long, and slow to render.
    # don't use any delay if using C stick.
    andi.   r7, r7, 0xF0 # deadzone
    bne     menuEndSub
    li      r3,  3
    stb     r3,  (menuJustMoved - mainLoop)(r14)
    b       menuEndSub

.objMenu_close:
    li      r3, 0
    sth     r3, (objMenuIdx - mainLoop)(r14)
    bl      returnToMainMenu
    b       menuEndSub

.objMenu_focus: # set the camera focus to the selected object
    # get the camera in r21
    LOADW   r21, pCamera
    li      r22, 0
    cmpwi   r21, 0
    beq     .objMenu_focus_noCamera
    stw     r18, 0xA4(r21)
    # since time is stopped we must manually update
    # the camera position.
    CALL    0x801030c0
    b       menuEndSub

.objMenu_focus_noCamera:
    li      r3, 0
    li      r4, 0xFD # "can't use that item"
    CALL    audioPlaySound
    b       menuEndSub


objMenuIdx: .short 0 # which object
objMenuState: .byte 0 # 0=list 1=view
.byte 0 # padding
