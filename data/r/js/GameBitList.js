import {E} from './Element.js';
import Table from './Table.js';
import {hex, makeCollapsibleList, makeDescriptionAndNotes} from './Util.js';
import {SaveGame} from './SaveGame.js';

export default class GameBitList {
    /** A page listing the texts in the game.
     */
    constructor(app) {
        this.app = app;
        this.element = E.span('loading');
        this._saveFile = null;
        this._saveGame = null;
        this._whichSave = 0; //XXX
    }

    getTitle() { return "GameBits" }

    async run() {
        this._makeFileSelect();
        this.refresh();
    }

    refresh() {
        this._makeTable();
        for(let [id, bit] of Object.entries(this.app.game.bits)) {
            this.tbl.add(this._makeRow(bit));
        }
        let _elem = E.span(this.fileSelect, this.tbl.element);
        this.element.replaceWith(_elem);
        this.element = _elem;
    }

    _makeFileSelect() {
        this._fileInput = E.input('savefile', {type:'file', id:'savefile'});
        this._fileSlotList = E.select('slotlist',
            E.option(null, '(no file loaded)'),
        );
        this.fileSelect = E.div('fileselect',
            E.h1(null, "Save File"),
            this._fileInput, this._fileSlotList);

        this._fileInput.addEventListener('change',
            async e => { await this._changeSaveFile() }, false);
    }

    async _changeSaveFile() {
        this._saveFile = new SaveGame();
        await this._saveFile.load(this._fileInput.files[0]);
        this._buildSaveSlotList();
        this._refreshSelectedSaveSlot();
    }

    _buildSaveSlotList() {
        let slots = E.select('file');
        const saves = this._saveFile.saves;
        for(let i=0; i<3; i++) {
            let name = "(empty)";
            let percent = '0';
            let time = '00h 00m 00s';
            if(saves[i].charState[0].maxHealth) {
                name = saves[i].saveFileName;
                percent = ((saves[i].completion / 187) * 100).toFixed(0).padStart(3, ' ');
                time = saves[i].playTime;
                time = (time / 3600).toFixed(0).padStart(2, '0') + 'h ' +
                    ((time/60) % 60).toFixed(0).padStart(2, '0') + 'm ' +
                    (time % 60).toFixed(0).padStart(2, '0') + 's';
            }
            slots.append(E.option(null, `#${i+1} ${name} ${percent}% ${time}`,
                {value:i}));
        }
        slots.addEventListener('change', e => {
            this._refreshSelectedSaveSlot();
        });
        this._fileSlotList.replaceWith(slots);
        this._fileSlotList = slots;
    }

    _refreshSelectedSaveSlot() {
        this._whichSave = this._fileSlotList.value;
        this._saveGame = this._saveFile.saves[this._whichSave];
        this.refresh();
    }

    _makeTable() {
        const v = this.app.game.version;
        this.tbl = new Table({
            columns: [
                {
                    displayName: "#",
                    name: 'id',
                    type: 'hex',
                    length: 4,
                }, {
                    displayName: "T",
                    name: 'table',
                    type: 'int',
                    makeElem: (val, td, bit) => {
                        td.classList.add(`bitTable${bit.table}`);
                        return td;
                    }
                }, {
                    displayName: "H",
                    name: 'hintId',
                    type: 'hex',
                    length: 2,
                }, {
                    displayName: "Size",
                    name: 'size',
                    type: 'int',
                }, {
                    displayName: "Offs",
                    name: 'offset',
                    type: 'hex',
                    length: 4,
                }, {
                    displayName: "Name",
                    name: 'name',
                    type: 'string',
                }, {
                    displayName: "Value",
                    name: 'value',
                    type: 'string',
                }, {
                    displayName: "Obj Refs",
                    name: 'objRefs',
                    type: 'string',
                    compareFunc: (a, b) => a.nObjRefs - b.nObjRefs,
                }, {
                    displayName: "Description",
                    name: 'description',
                    type: 'string',
                    makeElem: (val, td, row) => {
                        return E.td('string', makeDescriptionAndNotes(row),
                            E.div('hint', row.hint),
                        );
                    },
                },
            ]
        });
    }

    _makeRow(bit) {
        let hint = '';
        if(bit.hintId) {
            hint = makeCollapsibleList(bit.hint);
        }
        let objRefs = [];
        for(let r of bit.objRefs) {
            //to make these link to obj/map we need to re-parse the data
            //and include their IDs in the files, since names can be
            //ambiguous.
            objRefs.push(E.span('objref',
                E.span('objid', hex(r.objId,8)),
                E.span('mapname',   r.map), //XXX link to the map
                E.span('objname',   r.obj), //XXX link to the object
                E.span('hex',       r.param),
            ));
        }
        return {
            id:          bit.id,
            table:       bit.table,
            hintId:      bit.hintId,
            hint:        hint,
            size:        bit.size,
            offset:      bit.offset,
            name:        bit.name,
            value:       this._getValue(bit),
            description: bit.description,
            notes:       bit.notes,
            objRefs:     makeCollapsibleList(objRefs),
            nObjRefs:    bit.objRefs.length,
        };
    }

    _getValue(bit) {
        if(this._saveGame == null) return '';
        let table;
        switch(bit.table) {
            case 0: return ''; //temp bits not saved
            case 1: table = this._saveGame.gameBits1; break;
            case 2: table = this._saveGame.gameBits2; break;
            case 3: table = this._saveGame.gameBits3; break;
        }
        let offs = bit.offset;
        let val    = 0;
        for(let i=0; i<bit.size; i++) {
            val <<= 1;
            let byte = table[offs >> 3];
            if(byte & (1 << (7-(offs & 7)))) val |= 1;
            offs++;
        }

        return val.toString();
    }
}
