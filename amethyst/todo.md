to avoid crashing with multiple players, put a blr at 8016d9ec
it's crashing because it's trying to do something with the staff but they don't have one
- what if we spawn a staff for them and attach it?

# future versions
- keyboard stuff
- improve memory usage
	- does the staff swipe effect really need 180K!? really!?
	- Improve emergency free
		- free more things than only objects and expgfx
			- can we dump whichever map we're not currently in?
		- display a warning on screen if low on memory
			- maybe eg change the color of a HUD item?
- UI improvements
	- save Y button equip (see ybutton.s)
		- maybe we can just force the menu to be open and doing the equip?
	- fix stupid gametext texture generation thing (especially for uwu)
	- HUD improvements
		- Banjo-Kazooie style HUD - don't show counters unless relevant, or while paused
		- show more things (Tricky, enemies...) on minimap
		- make the PDA HUD switch to info mode when an object that has info is nearby, otherwise remain in map/compass mode
		- PDA info should show enemy HP and stats
	- make the WarpStone just YEET you instead of really warping
- Krystal stuff
	- height difference from Fox
		- fix camera placement in some scenes to look at her face
			- shouldn't the "focus points" handle this? maybe they're not being used?
	- staff impaling her on back
		- it happens to Fox too, just hidden by backpack
		- it's attach point #2 on the Sabre object, but just changing that isn't enough because it's actually split into two pieces when stored on the back.
			- can we move them together to hide that?
		- when entering first person mode the blend mode changes and it looks correct. can we mimic what's going on there?
			-it's changing her opacity. we can set it to 254 which does fix that but makes her look like some kind of Lovecraft abomination.
	- staff swipe animation (and hitbox?) when loaded actual Krystal object
		- what's causing that object to load anyway? can we just force it to use Sabre?
		- the hitboxes are still correct
	- if you roll against a corner while switching to/from the SharpClaw Disguise, you can get stuck in an animation/auto-walk loop until switching to Fox. This only happens to Krystal.
	- loincloth motion (maybe by loading actual Krystal anims?)
	- some attack hitboxes (maybe broken as Fox too?)
		- the sweeping kick to the head seems to not work, even though the hitboxes clearly connect...
	- the eyes go a bit wacky when getting a spell
	- import extra voices from kiosk version (are we sure these exist?)
- make some things better
	- Andross
		- be able to use your 1ups?
		- be less tedious in general
		- have other crew members (besides Falco) help by shooting and such?
	- move that one plant
	- put some more warps around to/from the hollow
	- make the test of fear less dumb
		- how much of the dumb is just input lag?
	- make Tricky less chatty
		- make him attack badguys without being told
	- make the fireball hitbox bigger and/or the switches
	- remove more pause menu voices
		- see Cheats.md for some more addresses there
- repack assets at higher compression ratio
	- strip unneeded textures from eg gamefront
	- might need to publish a script since this could result in a massive patch
		- or build a web-based patcher
- find a way to show previous frame buffer on BSOD
	- we can't just use one buffer and display the other, I guess you can't modify the one that's being shown.
	- the reflection texture just shows as garbage, dunno why
	- we need 0x96000 bytes to copy the buffer into
	- can we change it to a different graphic format and set up a new, smaller framebuffer?

# bugs
- backpack disappears from Krystal during cutscenes
- pushing block speed isn't affected by physics time scale
	- it's at least fast enough now that it probably doesn't matter
	- swimming behaves differently (currents seem to get stronger when speeding up time)
- when fur effect is on and you jump, there's some polygon glitching.
	- seems to be bone #8 that's not being set properly or something
	- affects both characters; reducing fur intensity avoids it
	- opening PDA menu during it will cause it to get much worse, then fix itself
	- happens even in original game with code to force fur effect
	- possibly related to glitches with Gecko handler? but happens without that too
- PDA debug:
	- inhibit Z button so we don't trigger first person view when using the menu if we opened it from the PDA item
	- ghost hitboxes
		- looks like the hitboxes really ARE moving. something else prevents them from being used.
- original game bugs
	- uwu suffers from missing letters due to stupid gametext thing
		- original Dino Language cheat is also affected by this
	- reflections

# ideas
- on BSOD, press Z to save
	- to current slot? but the save might be broken
	- to slot 3? but what if you're using it? especially since no copy function
	- to another memory card file?
		- probably simple enough by changing the game ID but how to load it? what if no room? etc
- PDA debug stuff
	- somehow be able to examine objects that aren't in the main list, eg the staff
	- show objects' names over the actual objects
	- be able to browse the map's romlist and spawn objects from it, bypassing state checks
	- toggle cheats: dino language, sepia...
	- draw the map grid borders as transparent walls in the world
- run in Wii mode?
	- maybe can modify Nintendon't to let it have access to additional RAM, CPU speed, devices
- if you're in the "stuck out of bounds" or "falling" state for a few seconds, warp back somewhere safe.
	- having the warp menu and free move makes this kinda redundant?
		- but this lets you escape the softlock without "cheats"
- a button to abort the current cutscene, or make all time commands super fast to skip to the end.
- auto aim - raycast to where we're aiming, and if an object such as a switch/enemy is nearby, snap to it
	- alternatively, have a hitbox extending from the staff outward a long way, and if it touches such an object, inhibit moving the staff a bit.
	- maybe just prevent it from moving back on its own?
	- maybe even just have an option where the aiming is relative, ie the stick moves the crosshair around relatively instead of setting its angle absolutely
	- if we had it able to do this we could also have debug things where you just point at objects.
	- if you're locked on to something, your fireballs home in on it. very noticeable during Galdon fight. can we use this?
- arbitrary aspect ratio setting
- hook doRumble to adjust intensity
- port Fox/Krystal's Assault outfits
	- import some models from elsewhere
	- Falco model from ending
	- original Krystal/Sabre from DP (maybe upscaled)
	- characters from Smash 64 and Melee?
- render silhouette of player above geometry like in new Mario games
- look into a way to prevent objects from despawning outside the map they're on
	- with the bike, changing map ID causes instacrash, even to FF (mapId at 811ea6ac)
	- change 80055f0c to 4800 to prevent unloading objects from other maps, then it won't crash, but things won't load correctly. probably out of memory
		- but even after unloading nearly everything, it still crashes...
	- the unload function actually checks for the bike but looks like it just unloads some extra things. this is after unloading the bike itself so can't just stick a return here.
- GameBit sets - load a file from disc/SD card and replace the current bit tables with it; export current tables

# fun/silly things
- Jesus Mode
- rideable Tricky, and/or spawn snow bike
	- you only need to load a map that has its assets, and spawn it with correct params. it's buggy (sometimes clipping into ground or getting stuck) but it works.
	- no way to get off properly, though... and where should it be left?
- change friction for various surfaces
- put something funny on the screen of Fox's computer during the Great Fox scene.
	- on the title screen it's the movie, but the "blank screen" texture behind it is also used in the other scenes.
- would be great to add cheat/debug functions to the pause menu
	- select a place on the world map and press a button to warp there
	- select an item in the stats screen and edit its quantity/toggle it
	- select a map on the list of maps and view it

# data mining
- rip sound effects
- rip models with texture, skeleton, animation
- examine md5.txt, see if any maps have different versions of files or unique, unused resources
- there's some more leftover stuff re: Tricky and abilities
  - the "decoy" voice clip exists, might be possible to call it
    - we can change one of the items in the C menu to 2 but it seems to not do anything besides the voice clip
  - according to old gametext, "bad guy alert" would make the staff glow red
    - one of the colors available is a red/orange, but it doesn't look like it's ever set to that by any existing code.
  - "hit protector" item in the demo version
    - this does absolutely squat when purchased (not even an animation) but code might remain?
- map debug - can we restore rendering the extra geometry?
- what controls Tricky's color?



# old notes
- figure out how to spawn the jet bike without the camera going nuts
  - when we use the real bike, the camera focus is on it, not us. but changing it to focus on us doesn't make any difference. nor does making it the focus when spawned elsewhere.
  - could be the way we spawn it. or the lack of IMSeqObject (01B9)
    - that just despawns immediately.
    - if we change its model to eg a fuel cell, the camera no longer goes nuts, but we can't use the bike. getting on just softlocks with a bunch of "sequence could not find object" errors.
  - also, there's no way to get off the bike...
  - should see what happens if, while on the bike, we use free move to go to some other map, or warp
    - it can't activate the warp pad, but should try going into a hole or directly to another map
    - if we go to the bottom of the Ice Mountain course and follow the trail the bike just disappears
      - if we warp the player back to the bike and set the focus to them, we can get on it a second time (which causes two WRONG WAY texts to appear), but then riding it across the map border causes a crash.
      - if we focus the player and run around while the bike is there, the camera goes crazy again, so maybe the problem is just that we're not in the right animation/state
        - also when walking around near the bike, the camera turns VERY fast.
        - setting the currently riding object to Tricky and state to 0x18 just crashes
        - spawning the bike and getting on sets us to state 0x18
        - changing the animation to 0xC9 doesn't help
        - could be because the bike isn't initialized correctly, same as when we spawn Fox and it has the wrong defNo
        - could also be the lack of checkpoint4
    - if we want to load the bike model we may need to also load the animation which means sorting ANIM.BIN.

Force Time of Day:
48000000 803dd12c # load po
ce000000 80008400 # check address range
1400020c 4728c000 # write to po
e2000001 00000000 # end

Disable HUD:
040753b8 4e800020
04075684 4e800020
04075a1c 4e800020
04075fc8 4e800020
04076510 4e800020
0407681c 4e800020
04076d78 4e800020
0407719c 4e800020
04079a64 4e800020
04079e64 4e800020
04075e8c 4e800020
