# Load Savegame patch

loadSaveGamePatch:
    # Patch the very end of objLoadPlayerFromSave
    # by patching here we can reuse the function's own stack
    # r28 = ObjInstance*
    lwz     r3, 0x7C(r28) # get models ptr
    lwz     r4, 0x00(r3)  # get Krystal model
    lwz     r5, 0x04(r3)  # get Fox model
    lwz     r6, 0x00(r4)  # Krystal header
    lwz     r7, 0x00(r5)  # Fox header
    lwz     r0, 0x6C(r7)  # Fox anim ptr
    stw     r0, 0x6C(r6)  # store to Krystal

    # switch to Krystal immediately.
    # otherwise, she'll be in a T pose when loading the game.
    # this is because the main loop patch won't switch the model
    # until one frame after the animations are initialized.
    # this didn't happen with the Gecko code, because it executes
    # every frame no matter what.
    # by doing this we ensure that the animations initialize for
    # Krystal, since we'll be swapping to her immediately anyway.
    # the swap happens before the screen fades in, so it's still
    # unnoticeable, and now she'll stand normally.
    li      r4, 0
    stb     r4, 0xAD(r28)

    # copy Fox's animIdxs to Krystal so the animations will play correctly
    # as well as field 0x80 which is right after
    li      r4, 0x70
    .loadSaveGamePatch_next:
        lwzx    r0,r4,r7 # get Fox animIdxs
        stwx    r0,r4,r6 # store to Krystal
        addi    r4,r4,4
        cmpwi   r4,0x84
        bne     .loadSaveGamePatch_next

    li   r0, 0 # replaced
    JUMP 0x802B6F2C, r11
