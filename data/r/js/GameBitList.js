import {E} from './Element.js';
import Table from './Table.js';
import {hex, makeList, makeCollapsibleList, makeDescriptionAndNotes} from './Util.js';

export default class GameBitList {
    /** A page listing the texts in the game.
     */
    constructor(app) {
        this.app = app;
        this._makeTable();
        this.element = this.tbl.element;
    }

    getTitle() { return "GameBits" }

    async run() {
        for(let [id, bit] of Object.entries(this.app.game.bits)) {
            this.tbl.add(this._makeRow(bit));
        }
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
                    displayName: "Max",
                    name: 'max',
                    type: 'hex',
                }, {
                    displayName: "Name",
                    name: 'name',
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
        if(bit.hasHint) {
            hint = this.app.game.texts[bit.hintId + 0xF4];
            if(hint) hint = hint.toList();
            else hint = `&lt;text 0x${hex(bit.hintId+0xF4,4)}&gt;`;
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
            max:         bit.max,
            name:        bit.name,
            description: bit.description,
            notes:       bit.notes,
            objRefs:     makeCollapsibleList(objRefs),
            nObjRefs:    bit.objRefs.length,
        };
    }
}
