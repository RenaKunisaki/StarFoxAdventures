.text
.include "common.s"

# Patch the very end of objLoadPlayerFromSave
GECKO_BEGIN_PATCH 0x802b6f28 # li r0, 0
# by patching here we can reuse the function's own stack
# r28 = ObjInstance*

lwz     r3, 0x7C(r28) # get models ptr
lwz     r4, 0x00(r3)  # get Krystal model
lwz     r5, 0x04(r3)  # get Fox model
lwz     r6, 0x00(r4)  # Krystal header
lwz     r7, 0x00(r5)  # Fox header
lwz     r0, 0x6C(r7)  # Fox anim ptr
stw     r0, 0x6C(r6)  # store to Krystal

# copy Fox's animIdxs to Krystal so the animations will play correctly
# as well as field 0x80 which is right after
li      r4, 0x70
.next:
    lwzx    r0,r4,r7 # get Fox animIdxs
    stwx    r0,r4,r6 # store to Krystal
    addi    r4,r4,4
    cmpwi   r4,0x84
    bne     .next

end:
    li      r0, 0 # replaced

GECKO_END_PATCH
