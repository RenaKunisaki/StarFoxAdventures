# Krystal mod v2.0
# Implemented as a patch file instead of a Gecko code.
# Improvements:
# - The SharpClaw Disguise now functions properly.
# - Switch character using the fancy new PDA menu.
# - Reduce load time and RAM usage by loading the assets from dedicated
#   files instead of animtest, and by removing a lot of repeated code.
# - Remove hardcoded file sizes.
# TODO:
# - Patch animations instead of using Fox's.
.set     DEBUG,0
.include "krystal/main.s"
.include "krystal/modelsbin.s"
.include "krystal/modelstab.s"
.include "krystal/tex1bin.s"
.include "krystal/tex1tab.s"
.include "krystal/savegame.s"
.include "krystal/mainloop.s"
.include "krystal/data.s"
