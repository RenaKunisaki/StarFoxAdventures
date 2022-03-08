import { assertType, hex } from "../../Util.js";
import Game from "../Game.js";

export default class SaveSlot {
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
