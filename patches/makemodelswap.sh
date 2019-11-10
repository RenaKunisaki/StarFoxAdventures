#!/bin/sh
./make.sh modelswap1
./make.sh modelswap2
./make.sh modelswap3
./make.sh modelswap4
rm modelswap.gecko
touch modelswap.gecko
cat modelswap?.gecko >> modelswap.gecko
rm modelswap?.gecko

# add "D-Down to Switch Character"
echo "283398d2 00000004" >> modelswap.gecko
echo "82200000 803428f8" >> modelswap.gecko
echo "86000000 000000ac" >> modelswap.gecko
echo "86410000 00010000" >> modelswap.gecko
echo "e2000000 80000000" >> modelswap.gecko
