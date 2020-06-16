# a few patches need to use global variables so that others
# (mainly the PDA Menu) kno where to find them.

.set ENABLE_FREE_MOVE,0x817FFFFE
.set CUR_CHAR_ADDR,0x817FFFFF # which character model to use
