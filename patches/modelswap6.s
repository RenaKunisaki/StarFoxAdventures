.text
.include "common.s"

# raw Gecko codes that work with the model swap.

# decrease size of heap 2 to make room
#echo "00024043 00000007" >> modelswap.gecko
#echo "00024063 00000007" >> modelswap.gecko
#echo "0202406a 00000352" >> modelswap.gecko
## increase size of heap 3
#echo "00024077 00000049" >> modelswap.gecko
#echo "00024097 00000049" >> modelswap.gecko
#echo "0202409e 00000244" >> modelswap.gecko
# unfortunately this isn't enough...
# because we weren't out of memory anyway, it was a bug in the patch.

GECKO_WRITE32 0x802b08d8, 0x38000001 # let Krystal use staff
GECKO_WRITE32 0x80295be0, 0x38600001 # let Krystal use map, comm, etc
# above patch seems to cause crashing on pause, so:
GECKO_WRITE32 0x8025c34c, 0x60000000 # fix crash on pause (not necessary?)

# D-Down to switch character
GECKO_IFEQ_16 buttonsJustPressed+2, PAD_BUTTON_DOWN # D-Down pressed?
    GECKO_LOAD_PO pPlayer # po = Player
    GECKO_IFEQ_16 0x44, 1, , , 1 # Player->objId == 1?
        # the Fox model on the title screen is actually assigned as
        # the Player object, but it's not the same object type; if we
        # change its model ID, the game will crash.
        # since we need D-pad Down to enter cheats, this won't do.
        GECKO_SET_GR 0, 0xAC, 1, 1 # gr0 = &Player->curModel
        GECKO_GR_OP 0, GECKO_OP_XOR, 0x00010000, 1 # *gr0 ^= 0x10000
        # also change the voice flag (which also changes backpack)
        GECKO_LOAD_GR_32 0, 0xB8, 1, 1 # r0 = Player->animState
        GECKO_GR_OP 0, GECKO_OP_ADD, 0x818 # gr0 += 0x818
        GECKO_GR_OP 0, GECKO_OP_XOR, 0x00000001, 1 # *gr0 ^= 1
GECKO_ENDIF 2, 0x8000 # end if x2, set ba=0x80000000
