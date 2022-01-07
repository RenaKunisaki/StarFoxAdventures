#include "main.h"

/* Instructions for translating:
 * 1. Copy this file, rename to the other language.
 * 2. Rename the variable from `strings_English`.
 * 3. Update `stringTables` in `strings.c`.
 * 4. Translate each string.
 * The strings must be in the same order as the English table.
 * It's OK if some are repeated, or empty.
 * Don't translate the comments.
 * Some strings are abbreviated to fit on the screen, so check if
 * your replacement fits as well.
 */

const char *strings_English[] = {
    "Address", //memory address
    "ADDR", //memory address
    "Attach Points", //show attachment points
    "Audio Settings", //change audio settings
    "Audio SFX", //sound effects
    "Audio Streams", //change volume of streaming audio
    "Auto", //automatic
    "Autosave", //save automatically
    "Backpack", //whether player wears a backpack
    "Back", //return to previous
    "Big", //PDA map size: big
    "Bird's Eye", //have camera look down from sky
    "Bit", //GameBit (not the same as binary bits)
    "Blur Filter", //enable a blur over the screen
    "Bounds", //boundaries
    "Bring To Player", //move selected object to player
    "Camera", //camera settings
    "Camera Control", //camera control mode
    "Camera Coords", //camera coordinates
    "Camera X", //camera control mode for X axis
    "Camera Y", //camera control mode for Y axis
    "CATID", //category ID
    "Cheats", //cheat functions
    "CHILD", //child object
    "Clear Restart Point", //clear the "restart point"
    "Color Filter", //change color filtering
    "Color Scale", //change color scaling
    "Controller", //which gamepad
    "Control Settings", //change controls
    "Crash Game", //cause a fatal error (for testing)
    "Cur Food", //current food/energy
    "Cur HP", //current health
    "Cur Lives", //current extra lives
    "Cur MP", //current magic power
    "CutScene Volume", //audio volume during scenes
    "Day", //day of month
    "Debug", //debug menu
    "Debug Objects", //show objects that are normally hidden
    "Debug Text", //show debugging text
    "DefNo", //definition number
    "Delete", //delete
    "Distance", //distance
    "DLL", //DLL (Dynamically Linked Library)
    "Edit", //edit
    "Edit File", //edit file
    "Edit Memory", //edit memory
    "Edit ObjDef", //edit object definition
    "Edit Player State", //edit player's state
    "Edit Tricky State", //edit Tricky's state
    "Edit State", //edit object's state
    "Effects", //effects
    "Environment", //environment (time, weather...)
    "EVENT", //EVENT
    "Exit", //exit this menu
    "Fast", //move faster
    "FILE", //FILE (on a computer)
    "Filter B", //filter setting for blue
    "Filter G", //filter setting for green
    "Filter R", //filter setting for red
    "First Person", //have camera look from player's head
    "FLAG", //FLAG (option)
    "Focus Points", //show camera focus points
    "FollowPtr", //follow pointer
    "Font Test", //display all fonts
    "FOV", //field of view
    "Fox", //Fox player character
    "FPS Meter", //Frames Per Second
    "Free", //let camera move anywhere
    "Free Move", //move the player anywhere
    "Frozen Baddies", //enemies can't move (effect of Ice Blast spell)
    "Fuel Cell Compass", //PDA mode: show nearby fuel cells
    "Fur Effect", //Fur effect
    "GameBit Changes", //indicate when a GameBit is changed
    "GameBits", //game flags
    "Game Settings", //change game settings
    "Game Speed", //change overall game speed
    "Game State", //change overall game state
    "Goto Last Save", //move to the last saved location
    "Go To", //move to this object
    "Goto Restart Point", //move to the "restart point"
    "Headphones", //sound mode: headphones
    "Heap Details", //show details about memory allocation
    "Heat Effect", //enable a heat wave effect on screen
    "Hitboxes", //show hit areas
    "HITS", //hitboxes
    "HUD", //heads-up display
    "ID", //ID number
    "Infinite HP", //infinite health
    "Infinite Lives", //infinite extra lives
    "Infinite Money", //infinite money (scarabs)
    "Infinite MP", //infinite magic
    "Infinite Tricky Energy", //infinite Tricky energy
    "Information", //PDA mode: show info about nearby objects
    "Interact Obj", //object the player is interacting with
    "Invert", //opposite direction (for camera)
    "Kill Player", //kill the player (for testing)
    "Krystal", //Krystal player character
    "Language", //change text language
    "Lights", //debug: examine lights
    "List", //open a list of objects
    "Load EnvFX", //load environment effects
    "Load  Flags", //flags to use when loading (extra space for alignment)
    "Load Map", //load a map's assets
    "Low HP Flash", //flash the HP meter when HP is low
    "Low HP Sound", //play a sound when HP is low=
    "Map Acts", //"act numbers" of each map
    "Map Grid", //show the map grid on-screen
    "Map ID", //ID number of map
    "Map", //map
    "Map Opacity", //opacity of PDA map
    "Map Size", //size of PDA map
    "Map States", //state of each map
    "Map Zoom", //zoom level of PDA map
    "Master", //master debug text enable
    "Max Food", //maximum food/energy
    "Max HP", //maximum health
    "Max Lives", //maximum extra lives
    "Max MP", //maximum magic power
    "Memory Stats", //show stats for memory allocation
    "Misc", //miscellaneous
    "MODEL", //3D model
    "Mode", //mode
    "Money", //current money (scarabs)
    "Monochrome", //show everything in monochrome
    "Mono", //sound mode: mono
    "Motion Blur", //enable a motion blur effect
    "Move", //move the cursor
    "MSGS", //message queue
    "Music Volume", //volume for background music
    "Name", //name
    "N/A", //not applicable
    "None", //none
    "NO OBJDEF", //no object definition data
    "Normal", //normal setting
    "OBJDEF", //object definition
    "Object", //game object
    "Objects", //show game objects
    "Obj ID", //object ID
    "OBJNO", //object number
    "Obj No.", //object number
    "Obj Seq", //show object sequences
    "Off", //off
    "On", //on
    "ORIG", //original position
    "OUT OF MEMORY", //out of memory
    "Page", //move to another page
    "Param", //parameter
    "Params", //parameters
    "PARENT", //parent object
    "Particle FX", //particle effects
    "PDA Menu", //PDA menu (main menu title)
    "PDA Settings", //PDA settings
    "Perf Meters", //performance meters
    "Performance", //show performance information
    "Pick Up", //pick up this object
    "Player Coords", //player coordinates
    "Player", //object menu instructions
    "Player Settings", //settings for player character
    "Player State", //player character's state
    "Player", //which player to use
    "Play Count", //number of times played
    "Play SFX", //play sound effect
    "Play Song", //play a song
    "Play Stream", //play audio stream
    "POS", //position
    "Reload Map", //reload the map
    "Render", //debug settings for rendering
    "Restart Point", //show the "restart point" coordinates
    "Ride", //start riding this object
    "RNG", //random number generator
    "Rumble Blur", //blur the screen when rumble is active
    "Rumble", //controller vibrate function
    "Run Seq", //run sequence
    "Saving...", //saving the game
    "SEQ", //sequence number
    "Set as Player", //make this object the player object
    "Set Camera Focus", //make the camera look at this object
    "Set Restart Point", //save currewnt position as "restart point"
    "Set Saved Pos", //save current position as "save point"
    "SFX Volume", //sound effect volume
    "Show", //show something
    "Skip", //skip this scene
    "SLOT", //slot number
    "Small", //PDA map size: small
    "Sorting...", //sorting the list
    "Sort", //Sort a list
    "Sound Mode", //choose a sound mode
    "Spawn Flags", //flags to use when spawning
    "Spawn Object", //spawn an object
    "Spawn", //perform the spawn
    "Spirit Vision", //show everything in effect used for Krazoa Spirit vision
    "STATE", //object state data
    "Stay", //have camera stay in place
    "Stereo", //sound mode: stereo
    "Stop All Sounds", //stop all sounds
    "Subtitles", //enable subtitles
    "Surround", //sound mode: surround
    "Sz", //2-letter abbreviation for Size
    "T", //1-letter abbreviation for Table
    "Table", //database table
    "Time", //time of day
    "Tricky", //the character named Tricky
    "UniqueID", //unique ID
    "Unk07", //unknown field 7
    "Unknown Points", //show ??? points
    "Unlock Everything", //give player all items, etc
    "Value", //value of a variable
    "Video Settings", //settings for display
    "View Fonts", //show all fonts
    "View Texts", //show all GameTexts
    "View Textures", //show all textures
    "Warp", //warp to another map
    "Widescreen", //enable widescreen display
    "Position", //coordinate
    NULL
};
