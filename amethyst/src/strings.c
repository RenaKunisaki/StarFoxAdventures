#include "main.h"
#include "revolution/os.h"

//XXX figure out some better way, maybe using the original GameText system

const char *strings_English[] = {
    "Address", //memory address
    "ADDR", //memory address
    "Attach Points", //show attachment points
    "Audio Settings",
    "Audio SFX", //sound effects
    "Audio Streams",
    "Auto", //automatic
    "Autosave", //save automatically
    "Backpack",
    "Back", //return to previous
    "Big",
    "Bird's Eye", //have camera look down from sky
    "Bit", //GameBit (not the same as binary bits)
    "Blur Filter",
    "Bounds", //boundaries
    "Bring To Player",
    "Camera",
    "Camera Control",
    "Camera Coords",
    "Camera X",
    "Camera Y",
    "CATID", //category ID
    "Cheats",
    "CHILD", //child object
    "Clear Restart Point", //clear the "restart point"
    "Color Filter",
    "Color Scale",
    "Controller", //gamepad
    "Control Settings",
    "Crash Game", //cause a fatal error
    "Cur HP", //current health
    "Cur Lives", //current extra lives
    "Cur MP", //current magic power
    "CutScene Volume",
    "Debug",
    "Debug Objects", //show objects that are normally hidden
    "Debug Text", //show debugging text
    "DefNo", //definition number
    "Delete",
    "Distance",
    "DLL",
    "Edit",
    "Edit File",
    "Edit Memory",
    "Edit ObjDef", //object definition
    "Edit Player State",
    "Edit State",
    "Effects",
    "Environment",
    "EVENT",
    "Exit",
    "Fast", //move faster
    "FILE",
    "Filter B",
    "Filter G",
    "Filter R",
    "First Person", //have camera look from player's head
    "FLAG",
    "Focus Points", //show camera focus points
    "FollowPtr", //follow pointer
    "FOV", //field of view
    "Fox", //Fox player character
    "FPS Meter", //Frames Per Second
    "Free", //let camera move anywhere
    "Free Move", //move the player anywhere
    "Frozen Baddies", //enemies can't move (effect of Ice Blast spell)
    "Fuel Cell Compass",
    "Fur Effect",
    "GameBit Changes",
    "GameBits", //game flags
    "Game Settings",
    "Game Speed",
    "Game State",
    "Go To",
    "Goto Last Save",
    "Goto Restart Point",
    "Headphones", //headphone sound mode
    "Heap Details",
    "Heat Effect",
    "Hitboxes", //show hit areas
    "HITS", //hitboxes
    "HUD", //heads-up display
    "ID",
    "Infinite HP", //infinite health
    "Infinite Lives",
    "Infinite Money",
    "Infinite MP", //infinite magic
    "Infinite Tricky Energy", //Tricky food
    "Information",
    "Interact Obj", //object the player is interacting with
    "Invert", //opposite direction (for camera)
    "Kill Player",
    "Krystal", //Krystal player character
    "Language",
    "Lights",
    "List",
    "Load  Flags", //flags to use when loading (extra space for alignment)
    "Low HP Flash", //flash the HP meter when HP is low
    "Low HP Sound", //play a sound when HP is low
    "Map", //game maps
    "Map Acts", //act numbers
    "Map Grid",
    "Map ID",
    "Map Opacity",
    "Map Size",
    "Map States", //state of each map
    "Map Zoom",
    "Master",
    "Max HP", //maximum health
    "Max Lives", //maximum extra lives
    "Max MP", //maximum magic power
    "Memory Stats",
    "Misc",
    "Mode",
    "MODEL",
    "Money", //current money (scarabs)
    "Monochrome", //show everything in monochrome
    "Mono", //mono sound mode
    "Motion Blur",
    "Move", //hexedit instructions
    "MSGS", //message queue
    "Music Volume",
    "Name",
    "N/A", //not applicable
    "None",
    "NO OBJDEF", //no object definition data
    "Normal",
    "OBJDEF", //object definition
    "Object",
    "Objects",
    "Obj ID", //object ID
    "Obj No.",
    "OBJNO", //object number
    "Off",
    "On",
    "ORIG", //original position
    "OUT OF MEMORY",
    "Page",
    "Param",
    "Params",
    "PARENT", //parent object
    "Particle FX", //particle effects
    "PDA Menu",
    "PDA Settings",
    "Perf Meters", //performance meters
    "Performance",
    "Pick Up",
    "Player Coords",
    "Player", //object menu instructions
    "Player Settings",
    "Player State",
    "Player", //which player to use
    "Play SFX", //play sound effect
    "Play Song",
    "Play Stream", //play audio stream
    "POS", //position
    "Reload Map",
    "Render",
    "Restart Point", //show the "restart point" coordinates
    "Ride", //start riding this object
    "RNG", //random number generator
    "Rumble", //controller vibrate function
    "Rumble Blur", //blur the screen when rumble is active
    "Run Seq", //run sequence
    "Saving...",
    "SEQ", //sequence
    "Set as Player", //make this object the player object
    "Set Camera Focus", //make the camera look at this object
    "Set Restart Point", //save currewnt position as "restart point"
    "Set Saved Pos", //save current position as "save point"
    "SFX Volume", //sound effect volume
    "Show", //show something
    "Skip", //skip this scene
    "SLOT", //slot number
    "Small",
    "Sort", //Sort a list
    "Sorting...",
    "Sound Mode", //choose a sound mode
    "Spawn", //perform the spawn
    "Spawn Flags", //flags to use when spawning
    "Spawn Object", //spawn an object
    "Spirit Vision", //show everything in effect used for Krazoa Spirit vision
    "STATE", //object state data
    "Stay", //have camera stay in place
    "Stereo", //stereo sound mode
    "Stop All Sounds",
    "Subtitles",
    "Surround", //surround sound mode
    "Sz", //2-letter abbreviation for Size
    "T", //1-letter abbreviation for Table
    "Table", //database table
    "Time", //time of day
    "Tricky Food", //Tricky's food/energy meter
    "Tricky", //the character named Tricky
    "UniqueID",
    "Unk07", //unknown field 7
    "Unknown Points", //show ??? points
    "Unlock Everything", //give player all items, etc
    "Value",
    "Video Settings",
    "View Textures",
    "Warp", //warp to another map
    "Widescreen",
    "X Position",
    "Y Position",
    "Z Position",
    NULL
};

//assign a table to each game language
//untranslated will use English until translation happens
const char **stringTables[] = {
    strings_English, //English
    strings_English, //French
    strings_English, //German
    strings_English, //Italian
    strings_English, //Japanese
    strings_English, //Spanish
};

const char* T(const char *s) {
    const char **table = stringTables[curLanguage];
    if(table == strings_English) return s;

    //look up the string in the English table (XXX cache?)
    for(int i=0; strings_English[i]; i++) {
        //string interning for constant strings
        if(strings_English[i] == s) return table[i];
    }

    //could loop again and strcmp here...
    OSReport("String not found in translation table: \"%s\"", s);
    return s;
}
