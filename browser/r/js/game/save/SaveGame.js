import { Header } from '../../types/gci.js';
import { assertType } from '../../Util.js';
import Game from '../Game.js';
import { ActualSaveData, CardFileStruct, SaveGameStruct } from './types.js';
import SaveSlot from './SaveSlot.js';

export class SaveGame {
    /** Reads the entire CardFileStruct from a File or Blob.
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

        if(file.size == CardFileStruct._size) {
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
        let view   = new DataView(buffer);
        let header = this.app.types.getType('gci.Header');
        console.assert(header);
        header = header.fromBytes(view);

        //let header = new Header(buffer);
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
        if(withIcons) this.data = new CardFileStruct(buffer).data;
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
