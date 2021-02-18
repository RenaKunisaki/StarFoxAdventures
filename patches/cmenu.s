# better C menu patch:
# removes useless items from the C menu.
.text
.include "common.s"

# define patches
patchList:
    PATCH_ID  "Cmenu  " # must be 7 chars
    # if removing spellstones we must create a way to place them
    # by eg pressing A at the opening
    #PATCH_HWORD 0x8031b1c0, 0x0096 # water spellstone 1
    #PATCH_HWORD 0x8031b1d0, 0x0096 # spellstone ?
    #PATCH_HWORD 0x8031b1e0, 0x0096 # spellstone ?
    #PATCH_HWORD 0x8031b1f0, 0x0096 # spellstone ?
    #PATCH_HWORD 0x8031b360, 0x0096 # fire spellstone 1
    #PATCH_HWORD 0x8031b370, 0x0096 # spellstone ?
    #PATCH_HWORD 0x8031b380, 0x0096 # fire spellstone 2
    #PATCH_HWORD 0x8031b390, 0x0096 # water spellstone 2
    PATCH_HWORD 0x8031b3e0, 0x0096 # fuel cells
    #PATCH_HWORD 0x8031b440, 0x0096 # give scarabs
    # same applies here as for stones
    #PATCH_HWORD 0x8031b450, 0x0096 # cheat token 0
    #PATCH_HWORD 0x8031b460, 0x0096 # cheat token 3
    #PATCH_HWORD 0x8031b470, 0x0096 # cheat token 2
    #PATCH_HWORD 0x8031b480, 0x0096 # cheat token 6
    #PATCH_HWORD 0x8031b490, 0x0096 # cheat token 4
    #PATCH_HWORD 0x8031b4a0, 0x0096 # cheat token 7
    #PATCH_HWORD 0x8031b4b0, 0x0096 # cheat token 1
    #PATCH_HWORD 0x8031b4c0, 0x0096 # cheat token 5
    #PATCH_HWORD 0x8031b530, 0x0096 # Open Portal spell
    #PATCH_HWORD 0x8031b540, 0x0096 # Staff Booster spell
    PATCH_END PATCH_FREE_AFTER_RUN

entry: # called as soon as our patch is loaded.
    blr # nothing to do here
.align 4
