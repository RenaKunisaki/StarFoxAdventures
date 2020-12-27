# checking for Fox
- one of the functions relating to the water spellstones checks if the player is Fox and can toggle that flag
    - may be a false positive if that flag means something else. not sure what object it's using.
    - it also checks player's magic
    - possibly not only used for stones
- pushable objects check if you're Krystal when you push them, but it's not clear what they do differently.
    - also when they load.
- some function related to carrying objects checks, but I can't find when the check is executed.
    - it's when you enter the main area of the CloudRunner Dungeon
    - if you aren't, you can't pick up the blocks
- the death gas code checks, but it seems to never execute
    - it's the gas in the power chamber, not cape claw
    - if you're Fox, it doesn't hurt you
    - this check must not actually be "playerIsFox" because these make no sense
- the guard robots check but I can't see any difference
    - is this check also checking for being disguised? but they aren't reacting to me at all.
    - I think it is. they shoot at me if I'm *not* Fox?
- when the LightFoot is being attacked in Cape Claw, if you're not Fox, the one SharpClaw won't attack you, but you also can't hurt him.

most of these actually are checking if you're disguised.
