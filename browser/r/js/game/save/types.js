import Struct from '../../lib/Struct.js';
import { Vec3f, Vec3i } from '../../types/common.js';

//Player character saved position
export const PlayerCharPos = Struct(
    [Vec3f, 'pos'],
    ['b',   'rotX'],
    ['b',   'mapLayer'],
    ['b',   'mapNo'],
    ['b',   'unk0F'], //padding?
);

//Player character saved state
export const PlayerCharState = Struct(
    ['b', 'curHealth'],
    ['b', 'maxHealth'],
    ['b', 'unk02'],
    ['b', 'unk03'],
    ['H', 'curMagic'],
    ['H', 'maxMagic'],
    ['B', 'money'],
    ['B', 'curBafomDads'],
    ['B', 'maxBafomDads'],
    ['b', 'unk0B'], //padding?
);

//High score table
export const SavedHighScore = Struct(
    ['I',  'score'],
    ['4s', 'name'],
    //XXX different for Japanese?
);

//Settings common to all save files
export const SaveGameSettings = Struct(
    ['b', 'exists'],
    ['B', 'unused01'], //Amethyst: extra options
    ['B', 'bSubtitlesOn'],
    ['B', 'unusedHudSetting'], //Amethyst: camera options; copied to a never-read variable
    ['B', 'unusedCameraSetting'], //copied to bitmask 0x18 of camera->flags_0x141
    ['B', 'unused05'],
    ['B', 'bWidescreen'],
    ['B', 'unused07'], //Amethyst: current character
    ['B', 'bRumbleEnabled'],
    ['B', 'soundMode'],
    ['b', 'musicVolume'],
    ['b', 'sfxVolume'],
    ['b', 'cutsceneVolume'],
    ['B', 'unused0D'], //Amethyst: options
    ['H', 'unused0E'], //Amethyst: FOV, map opacity
    ['I', 'unlockedCheats'],
    ['I', 'activeCheats'],
    ['I', 'unk18'],
);

//Fields common to all save files
export const SaveSettingsAndScores = Struct(
    [SaveGameSettings,  'settings'],
    [SavedHighScore[5], 'scoresToPlanet'],
    [SavedHighScore[5], 'scoresDarkIce'],
    [SavedHighScore[5], 'scoresCloudRunner'],
    [SavedHighScore[5], 'scoresWallCity'],
    [SavedHighScore[5], 'scoresDragonRock'],
);

//Environment state
export const SaveGameEnvState = Struct(
    ['f',      'timeOfDay'],
    ['5h',     'envFxActIdx'],
    ['3h',     'envFxActIdx2'],
    [Vec3i[3], 'skyObjPos'],
    ['2i',     'unk38'],
    ['b',      'flags'],
    ['3b',     'skyObjIdx'],
);

//Saved object position (rarely used)
export const SaveGameObjectPos = Struct(
    ['i',   'id'],  //object unique ID
    [Vec3f, 'pos'],
);

//One in-game save file
export const SaveGameStruct = Struct(
    [PlayerCharState[2],   'charState'],
    ['b',                  'trickyEnergy'],
    ['b',                  'maxTrickyEnergy'],
    ['B',                  'trickyPlayCount'],
    ['b',                  'unk1B'],
    ['4s',                 'name'], //3-letter file name
    ['b',                  'character'], //current character; 0:Krystal 1:Fox
    ['B',                  'flags21'],
    ['B',                  'flags22'],
    ['B',                  'unk23'],
    ['324B',               'gameBits2'],
    [SaveGameObjectPos[63],'objs'],
    ['5B',                 'texts'],
    ['B',                  'completion'], //out of 187
    ['B',                  'numTexts'],
    ['B',                  'unk55F'],
    ['f',                  'playTime'], //frame count
    ['116B',               'gameBits1'],
    ['172B',               'gameBits3'],
    [PlayerCharPos[2],     'charPos'],
    ['h',                  'unk6A4'], //related to camera
    ['h',                  'unk6A6'],
    [SaveGameEnvState,     'env'],
);

//The other data used by the GC's menu
export const CardIconsAndText = Struct(
    ['32s',   'fileName'],    //always "Star Fox Adventures" or Japanese
    ['32s',   'comment'],     //always "Dinosaur Planet" or "STARFOX ADVENTURES"
    ['6144B', 'opening_bnr'], //opening.bnr graphic
    ['4096B', 'icon_img'], //animated icon frames (4x1024)
    ['512B',  'icon_pal'],    //icon palette
    ['I',     'cksum1'],      //checksum
    ['I',     'cksum2'],      //inverse checksum
    ['2I',    'unk2A48'], //probably checksums
);

export const ActualSaveData = Struct(
    [SaveGameStruct[3],     'saves'],
    [SaveSettingsAndScores, 'global'],
)

//The entire file on the memory card
export const CardFileStruct = Struct(
    [CardIconsAndText,      'icons'],
    [ActualSaveData,        'data'],
    ['I',                   'cksum1'],
    ['I',                   'cksum2'],
);
