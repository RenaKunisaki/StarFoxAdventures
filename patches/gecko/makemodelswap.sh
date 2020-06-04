#!/bin/sh
rm modelswap?.gecko
./make.sh modelswap1
./make.sh modelswap2
./make.sh modelswap3
./make.sh modelswap4
./make.sh modelswap5
./make.sh modelswap6
rm modelswap.gecko
touch modelswap.gecko
cat modelswap?.gecko >> modelswap.gecko
#rm modelswap?.gecko
