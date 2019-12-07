.text
.include "common.s"

# patch map unloading; ensure our model/textures remain loaded.
# this prevents a case where one slot, containing the resources,
# gets unloaded, and nothing new gets loaded because the destination
# is the same map ID.
GECKO_BEGIN_PATCH 0x80043bd4 # sth r4, 0x68c8(r3)
# r6 = file ID being unloaded
# r3 = address of file pointer
# r4 = -1
# free: r0, r3

.set TEXTURE_ID,0x724

start:
    sth     r4, 0x68c8(r3) # replaced
    LOADWH  r0, fileBuffers

    cmpwi   r6, 0x20 # TEX1.bin   slot 1
    beq     do_tex1_slot1

    cmpwi   r6, 0x2B # MODELS.bin slot 1
    beq     do_models_slot1

    cmpwi   r6, 0x46 # MODELS.bin slot 2
    beq     do_models_slot2

    cmpwi   r6, 0x4B # TEX1.bin   slot 2
    bne     end

do_tex1_slot2:
    # TEX1.bin slot 2 is unloaded
    # look in TEX1.tab slot 2, check if our textures are there.
    LOADWL  r4, (fileBuffers+(0x4C*4)), r0

    # point r0 to the map ID of the other slot
    # we'll clear it if necessary
    # loadedFileMapIds@h == fileBuffers@h so no need to change here
    ori     r0, r0, ((loadedFileMapIds@l) + (0x20*4))
    b       do_tex1

do_tex1_slot1:
    # TEX1.bin slot 1 is unloaded
    # look in TEX1.tab slot 1, check if our textures are there.
    LOADWL  r4, (fileBuffers+(0x21*4)), r0
    ori     r0, r0, ((loadedFileMapIds@l) + (0x4B*4))

do_tex1:
    cmpwi   r4, 0
    beq     end  # no table here, so can't be
    lbz     r4, (TEXTURE_ID*4)(r4)
    andi.   r4, r4, 0xFE
    beq     end # not here.
    # else, this is our texture
    # so ensure the opposite slot is reloaded
    b       do_unload


do_models_slot1:
    # same deal here, check this slot's table,
    # and if our model's there, unload the other slot

do_models_slot2:


do_unload:
    li      r4, 0xFFFF
    sth     r4, 0(r0)

end:

GECKO_END_PATCH
