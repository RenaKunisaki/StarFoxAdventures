import { assertType } from '../../Util.js';
import Game from '../Game.js';
import SaveSlot from './SaveSlot.js';

//struct types
let Header, ActualSaveData, CardFileStruct, SaveGameStruct;

export class SaveGame {
    /** Reads the entire CardFileStruct from a File or Blob.
     */
    constructor(game) {
        this.game      = assertType(game, Game);
        this.app       = game.app;
        this.gciHeader = null;

        Header = this.app.types.getType('gci.Header');
        ActualSaveData = this.app.types.getType('sfa.save.ActualSaveData');
        CardFileStruct = this.app.types.getType('sfa.save.CardFileStruct');
        SaveGameStruct = this.app.types.getType('sfa.save.SaveGameStruct');
    }

    async load(file, version='U0') {
        /** Load a save file.
         *  @param {BinaryFile} file The file to load.
         *  @param {String} version The game version this file belongs to.
         */
        this._file    = file;
        this._version = version; //game version
        const buffer  = await this._file.arrayBuffer();
        const view    = new DataView(buffer);

        if(file.size == CardFileStruct.size) {
            //parse raw save file
            this._parseSave(view);
        }
        else if(file.size >= 512*1024) {
            //parse RAW memory card image
            throw new Error("RAW memory card image not supported yet");
        }
        else if(file.size >= 0x6040) {
            await this._parseGci(view);
        }
        else if(file.size == 1772 || file.size == 3952 || file.size == 6144) {
            //3952: save1.bin, a single save slot + a bunch of zeros
            //1772: save[2..5].bin, a single save slot
            //6144: savegame.bin, no idea
            this._parseOneSlot(view);
        }
        else {
            throw new Error("File is too small to be a valid savegame");
        }
    }

    getGameBits() {
        for(let slot of this.saves) slot.getGameBits();
    }

    async _parseGci(view) {
        /** Parse GCI image (raw memory card sectors of only this game).
         *  @param {DataView} view The view to read from.
         */
        let header     = Header.fromBytes(view);
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
        this._parseSave(new DataView(view.buffer.slice(0x40)), true);
    }

    _parseSave(view, withIcons) {
        /** Parse the actual save data.
         *  @param {DataView} view The view to read from.
         *  @param {bool} withIcons Whether the view includes the icon data.
         */
        if(withIcons) this.data = CardFileStruct.fromBytes(view).data;
        else this.data = ActualSaveData.fromBytes(view);
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

    _parseOneSlot(view) {
        /** Parse a single save slot, from debug savegame/saveN.bin.
         *  @param {DataView} view The view to read from.
         */
        this.data   = null;
        this.global = null;
        this.saves  = [
            new SaveSlot(this.game, 0, SaveGameStruct.fromBytes(view)),
        ];
    }
}
