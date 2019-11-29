#!/bin/sh
rm modelswap?.gecko
./make.sh modelswap1
./make.sh modelswap2
./make.sh modelswap3
./make.sh modelswap4
./make.sh modelswap5
#./make.sh modelswap6
rm modelswap.gecko
touch modelswap.gecko
cat modelswap?.gecko >> modelswap.gecko
#rm modelswap?.gecko

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

# add patch to let Krystal use staff
echo "042b08d8 38000001" >> modelswap.gecko
# this patch enables more things, like the map,
# but crashes when you pause
echo "04295be0 38600001" >> modelswap.gecko
# this fixes the crash but I don't know what it's doing
# I have graphical glitches on the pause menu even without patches
echo "0425c34c 60000000" >> modelswap.gecko

# add "D-Down to Switch Character"
echo "283398d2 00000004" >> modelswap.gecko # button pressed?
echo "48000000 803428f8" >> modelswap.gecko # po = Player*
# check player object ID so we don't crash on title screen
# the Fox model there is actually assigned as the Player object,
# but it's not the same object type, so changing its model = crash.
# since we need D-pad Down to enter cheats, this won't do.
echo "38000044 00000001" >> modelswap.gecko # Player->objId == 1?
echo "90010000 000000ac" >> modelswap.gecko # r0 = &Player->curModel
echo "86410000 00010000" >> modelswap.gecko # *r0 ^= 1
echo "92210000 000000b8" >> modelswap.gecko # r0 = Player->animState
echo "86000000 00000818" >> modelswap.gecko # r0 += r0+0x818
echo "86410000 00000001" >> modelswap.gecko # *r0 ^= 1
echo "e2000002 80000000" >> modelswap.gecko # end if (x2)
