import Struct from './Struct.js';

//GCI header
export const GciHeader = Struct({
    'gameCode':     ['4s',  0x00],
    'company':      ['2s',  0x04],
    'pad06':        ['b',   0x06],
    'bannerFmt':    ['b',   0x07],
    'filename':     ['32s', 0x08],
    'lastModified': ['I',   0x28], //seconds since 2000-01-01 00:00:00
    'iconAddr':     ['I',   0x2C], //where in the file to find the icon image
    'iconFmt':      ['H',   0x30],
    'iconSpeed':    ['H',   0x32],
    'permissions':  ['B',   0x34],
    'copyCount':    ['B',   0x35],
    'block':        ['H',   0x36], //which block this file begins at, plus 5
    'length':       ['H',   0x38],
    'pad3A':        ['H',   0x3A],
    'commentAddr':  ['H',   0x3C], //where in the file to find the comment string
});

//XXX RAW memory card struct

export const Vec3f = Struct({
    'x':      ['f', 0x00],
    'y':      ['f', 0x04],
    'z':      ['f', 0x08],
});
export const Vec3i = Struct({
    'x':      ['i', 0x00],
    'y':      ['i', 0x04],
    'z':      ['i', 0x08],
});

export const PlayerCharPos = Struct({
    'pos':      {type:Vec3f, offset:0x00},
    'rotX':     ['b',   0x0C],
    'mapLayer': ['b',   0x0D],
    'mapNo':    ['b',   0x0E],
    'unk0F':    ['b',   0x0F], //padding?
});

export const PlayerCharState = Struct({
    'curHealth':    ['b', 0x00],
    'maxHealth':    ['b', 0x01],
    'unk02':        ['b', 0x02],
    'unk03':        ['b', 0x03],
    'curMagic':     ['H', 0x04],
    'maxMagic':     ['H', 0x06],
    'money':        ['B', 0x08],
    'curBafomDads': ['B', 0x09],
    'maxBafomDads': ['B', 0x0A],
    'unk0B':        ['b', 0x0B], //padding?
});

export const SavedHighScore = Struct({
    'score': ['I',  0x00],
    'name':  ['4s', 0x04],
});

export const SaveGameSettings = Struct({
    'exists':              ['b', 0x00],
    'unused01':            ['B', 0x01], //Amethyst: extra options
    'bSubtitlesOn':        ['B', 0x02],
    'unusedHudSetting':    ['B', 0x03], //Amethyst: camera options; copied to a never-read variable
    'unusedCameraSetting': ['B', 0x04], //copied to bitmask 0x18 of camera->flags_0x141
    'unused05':            ['B', 0x05],
    'bWidescreen':         ['B', 0x06],
    'unused07':            ['B', 0x07], //Amethyst: current character
    'bRumbleEnabled':      ['B', 0x08],
    'soundMode':           ['B', 0x09],
    'musicVolume':         ['b', 0x0A],
    'sfxVolume':           ['b', 0x0B],
    'cutsceneVolume':      ['b', 0x0C],
    'unused0D':            ['B', 0x0D], //Amethyst: options
    'unused0E':            ['H', 0x0E], //Amethyst: FOV, map opacity
    'unlockedCheats':      ['I', 0x10],
    'activeCheats':        ['I', 0x14],
    'unk18':               ['I', 0x18],
});

export const SaveSettingsAndScores = Struct({
    'settings':         {type:SaveGameSettings,        offset:0x0000},
    'scoresToPlanet':   {type:SavedHighScore, count:5, offset:0x001C},
    'scoresDarkIce':    {type:SavedHighScore, count:5, offset:0x0044},
    'scoresCloudRunner':{type:SavedHighScore, count:5, offset:0x006C},
    'scoresWallCity':   {type:SavedHighScore, count:5, offset:0x0094},
    'scoresDragonRock': {type:SavedHighScore, count:5, offset:0x00BC},
});

export const SaveGameEnvState = Struct({
    'timeOfDay':    ['f',   0x00],
    'envFxActIdx':  ['5h',  0x04],
    'envFxActIdx2': ['3h',  0x0E],
    'skyObjPos':    {type:Vec3i, count:3, offset:0x14},
    'unk38':        ['2i',  0x38],
    'flags':        ['b',   0x40],
    'skyObjIdx':    ['3b',  0x41],
});

export const SaveGameObjectPos = Struct({
    'id':  ['i',   0x00],
    'pos': {type:Vec3f, offset:0x04},
});

export const SaveGameStruct = Struct({
    'charState':       {type:PlayerCharState, count:2, offset:0x00},
    'trickyEnergy':    ['b',    0x18],
    'maxTrickyEnergy': ['b',    0x19],
    'trickyPlayCount': ['B',    0x1A],
    'unk1B':           ['b',    0x1B],
    'saveFileName':    ['4s',   0x1C],
    'character':       ['b',    0x20], //0:Krystal 1:Fox
    'flags21':         ['B',    0x21],
    'flags22':         ['B',    0x22],
    'unk23':           ['B',    0x23],
    'gameBits2':       ['324B', 0x24],
    'objs':            {type:SaveGameObjectPos, count:63, offset:0x168},
    'texts':           ['5B',   0x558],
    'completion':      ['B',    0x55D], //out of 187
    'numTexts':        ['B',    0x55E],
    'unk55F':          ['B',    0x55F],
    'playTime':        ['f',    0x560], //frame count
    'gameBits1':       ['116B', 0x564],
    'gameBits3':       ['172B', 0x5D8],
    'charPos':         {type:PlayerCharPos, count:2, offset:0x684},
    'unk6A4':          ['h',    0x6A4], //related to camera
    'unk6A6':          ['h',    0x6A6],
    'env':             {type:SaveGameEnvState, offset:0x6A8},
});

export const CardIconsAndText = Struct({
    'fileName':    ['32s',   0x00], //always "Star Fox Adventures"
    'comment':     ['32s',   0x20], //always "Dinosaur Planet"
    'opening_bnr': ['6144B', 0x40], //opening.bnr graphic
    'icon_img':    {type:'1024B', count:4, offset:0x1840}, //animated icon frames
    'icon_pal':    ['512B',  0x2840], //icon palette
    'cksum1':      ['I',     0x2A40], //checksum
    'cksum2':      ['I',     0x2A44], //inverse checksum
});

//entire save file on memory card
export const SaveDataStruct = Struct({
    'icons':    {type:CardIconsAndText,        offset:0x0000},
    'unk2A48':  ['2I',                                0x2A48], //checksums?
    'saves':    {type:SaveGameStruct, count:3, offset:0x2A50},
    'global':   {type:SaveSettingsAndScores,   offset:0x3F14},
    'cksum1':   ['I',                                 0x3FF8],
    'cksum2':   ['I',                                 0x3FFC],

});

export class SaveGame {
    async load(file) {
        //file: File or Blob
        this._file = file;

        if(file.size == SaveDataStruct._size) {
            //parse raw save file
        }
        else if(file.size >= 512*1024) {
            //parse RAW memory card image
            throw "RAW memory card image not supported yet";
        }
        else if(file.size >= 0x6040) {
            await this._parseGci();
        }
        else {
            throw "File is too small to be a valid savegame";
        }
    }

    async _parseGci() {
        //parse GCI image (raw memory card sectors of only this game)
        let buffer = await this._file.arrayBuffer();
        let header = new GciHeader(buffer);
        console.log("GCI Header:", header._toString());
        if(header.gameCode != 'GSAE' || header.company != '01') {
            throw "Not a Star Fox Adventures save file";
        }
        //ignore the block field in the header.
        this._parseSave(buffer.slice(0x40));
    }

    _parseSave(buffer) {
        let data = new SaveDataStruct(buffer);
        //console.log("Save data:", data._toString());

        this.global = data.global;
        this.saves  = data.saves;
        console.log("Save 1:", this.saves[0]._toString());
        //console.log("Save 2:", this.saves[1]._toString());
        //console.log("Save 3:", this.saves[2]._toString());
    }
}
