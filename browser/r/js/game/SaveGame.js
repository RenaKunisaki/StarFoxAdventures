import Struct from '../lib/Struct.js';
import { Header } from '../types/gci.js';
import { Vec3f, Vec3i } from '../types/common.js';
import { assertType, hex } from '../Util.js';
import Game from './Game.js';

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
export const SaveDataStruct = Struct(
    [CardIconsAndText,      'icons'],
    [ActualSaveData,        'data'],
    ['I',                   'cksum1'],
    ['I',                   'cksum2'],
);

export class SaveSlot {
    /** One of the three save slots in the save file.
     */
    constructor(game, idx, save) {
        this.game  = assertType(game, Game);
        this.app   = game.app;
        this.index = idx;  //slot number
        this._save = save; //SaveGameStruct
        console.assert(save);

        this.gameBits = null;

        //get other fields (HACK)
        const fields = ['trickyEnergy', 'maxTrickyEnergy', 'trickyPlayCount',
            'unk1B', 'character', 'flags21', 'flags22', 'unk23', 'texts',
            'numTexts', 'unk55F', 'unk6A4', 'unk6A6', 'charState', 'charPos'];
        for(let f of fields) {
            this[f] = this._save[f];
        }
    }

    getGameBits() {
        //called later once bits have been loaded
        console.assert(this.game.bits);
        this.gameBits = {};
        for(let [id, bit] of Object.entries(this.game.bits)) {
            let name = bit.name || `_${hex(id,4)}`;
            let tbl=null;
            let val=0;
            switch(bit.table) {
                case 0: val = null; break; //temporary table, not saved
                case 1: tbl = this._save.gameBits1; break;
                case 2: tbl = this._save.gameBits2; break;
                case 3: tbl = this._save.gameBits3; break;
            }
            if(tbl != null) { //extract value
                for(let i=bit.offset; i<bit.offset+bit.size; i++) {
                    val <<= 1;
                    if(tbl[i >> 3] & (1 << (7-(i & 7)))) val |= 1;
                }
            }
            this.gameBits[name] = val;
            this.gameBits[id] = val;
        }
    }

    [Symbol.toPrimitive](hint) {
        //called to convert this object to a primitive type
        //hint is 'number', 'string', 'default'
        if(hint == 'string') {
            let res = `Save Slot #${this.index+1}: `;
            if(this.isEmpty) res += "Empty";
            else res += `"${this.name}"`;
            return `[${res}]`;
        }
        return null;
    }

    get isEmpty()  { return this._save.name == ""; }
    get name()     { return this._save.name; }
    get playTime() { //eg "00h 00m 00s" or "999999h 59m 59s"
        let time = this._save.playTime / 60; //time is in frames
        return (time / 3600).toFixed(0).padStart(2, '0') + 'h ' +
            ((time/60) % 60).toFixed(0).padStart(2, '0') + 'm ' +
            (time % 60).toFixed(0).padStart(2, '0') + 's';
    }
    get completion() { //0 (0%) to 1 (100%)
        return this._save.completion / 187;
    }
    get player() {
        const names = ["Krystal", "Fox"];
        let p = this._save.character;
        return {
            which: p,
            name:  names[p],
            state: this._save.charState[p],
            pos:   this._save.charPos[p],
        };
    }
}

export class SaveGame {
    /** Reads the entire SaveDataStruct from a File or Blob.
     */
    constructor(game) {
        this.game      = assertType(game, Game);
        this.app       = game.app;
        this.gciHeader = null;
    }

    async load(file, version='U0') {
        this._file    = file;
        this._version = version; //game version
        const buffer  = await this._file.arrayBuffer();

        if(file.size == SaveDataStruct._size) {
            //parse raw save file
            this._parseSave(buffer);
        }
        else if(file.size >= 512*1024) {
            //parse RAW memory card image
            throw new Error("RAW memory card image not supported yet");
        }
        else if(file.size >= 0x6040) {
            await this._parseGci(buffer);
        }
        else if(file.size == 1772 || file.size == 3952 || file.size == 6144) {
            //3952: save1.bin, a single save slot + a bunch of zeros
            //1772: save[2..5].bin, a single save slot
            //6144: savegame.bin, no idea
            let buffer = await this._file.arrayBuffer();
            this._parseOneSlot(buffer);
        }
        else {
            throw new Error("File is too small to be a valid savegame");
        }
    }

    getGameBits() {
        for(let slot of this.saves) slot.getGameBits();
    }

    async _parseGci() {
        //parse GCI image (raw memory card sectors of only this game)
        let buffer = await this._file.arrayBuffer();
        let header = new Header(buffer);
        this.gciHeader = header;
        console.log("GCI Header:", header);
        if(!header.gameCode.startsWith('GSA') || header.company != '01') {
            throw new Error("Not a Star Fox Adventures save file" +
                ` (game ID: "${header.gameCode}${header.company}")`);
        }
        switch(header.gameCode[3]) {
            //no way to find revision from just this header.
            //XXX these letters should match. this is confusing.
            case 'E': this._version = 'U0'; break; //US
            case 'P': this._version = 'E0'; break; //EU
            case 'J': this._version = 'J0'; break; //JP
            default: throw new Error(`Unknown version '${header.gameCode}'`);
        }
        //ignore the block field in the header.
        this._parseSave(buffer.slice(0x40), true);
    }

    _parseSave(buffer, withIcons) {
        //parse the actual save data
        if(withIcons) this.data = new SaveDataStruct(buffer).data;
        else this.data = new ActualSaveData(buffer);
        //console.log("this.data=", this.data);
        //console.log("this.data.global.settings.exists=", this.data.global.settings.exists);
        //console.log("this.data.saves=", this.data.saves);
        //console.log("this.data.saves[0]=", this.data.saves[0]);
        this.global = this.data.global;
        this.saves  = [
            new SaveSlot(this.game, 0, this.data.saves[0]),
            new SaveSlot(this.game, 1, this.data.saves[1]),
            new SaveSlot(this.game, 2, this.data.saves[2]),
        ];
    }

    _parseOneSlot(buffer) {
        //parse a single save slot, from debug savegame/saveN.bin
        this.data   = null;
        this.global = null;
        this.saves  = [
            new SaveSlot(this.game, 0, new SaveGameStruct(buffer)),
        ];
    }
}
