import { E, clearElement } from "../../lib/Element.js";
import { CollapseList, downloadXml, hex } from "../../Util.js";
import Table from "./Table.js";

const XML = 'http://www.w3.org/1999/xhtml';

export default class GameBits {
    /** Displays table of GameBits.
     */
    constructor(app) {
        this.app = app;
        this.element = document.getElementById('tab-gameBits');
        this.app.onSaveSlotChanged(slot => this.refresh());
        this.app.onIsoLoaded(iso => this.refresh());

        this.btnSave = E.button('save', "Save");
        this.btnSave.addEventListener('click', e => this._save());
    } //constructor

    refresh() {
        this.app.game.getBits().then(e => {
            let tbl = this._makeTable();
            for(let [id, bit] of Object.entries(this.app.game.bits)) {
                tbl.add(this._makeRow(bit));
            }
            if(this.app.saveSlot == null) tbl.hideColumn('value');
            else tbl.showColumn('value');
            const elem = E.div('gameBits', tbl.element);
            clearElement(this.element).append(this.btnSave, elem);
        });
    }

    _makeTable() {
        return new Table({columns: [
            {displayName:"#", name:'id',    type:'hex', length: 4,
                makeElem: (val, td, row) => {
                    if(row.bit.offset == 0 && row.bit.table == 0) {
                        td.classList.add('deleted');
                    }
                    return td;
                },
            },
            {displayName:"T", name:'table', type:'int',
                makeElem: (val, td, row) => {
                    td.classList.add(`bitTable${row.table}`);
                    return td;
                },
            },
            {displayName:"H",        name:'hintId',  type:'hex', length: 2},
            {displayName:"Size",     name:'size',    type:'int'},
            {displayName:"Offs",     name:'offset',  type:'hex', length: 4},
            {displayName:"Name",     name:'name',    type:'string',
                onEdit: (row, col, e, td) => {
                    row.bit.name = td.innerText;
                    this.btnSave.classList.add('unsaved');
                },
            },
            {displayName:"Value",    name:'value',   type:'string', classes:'int'},
            {displayName:"Obj Refs", name:'objRefs', type:'string',
                compareFunc: (a, b) => {
                    let ra = a.objRefs[0];
                    let rb = b.objRefs[0];
                    ra = ra ? `${ra.map} ${ra.objNo} ${ra.objId} ${ra.param}` : '';
                    rb = rb ? `${rb.map} ${rb.objNo} ${rb.objId} ${rb.param}` : '';
                    if(ra > rb) return  1;
                    if(ra < rb) return -1;
                    return 0;
                },
                makeElem: (val, td, row) => this._makeObjRefsElem(row),
            },
            {displayName:"Description", name:'description', type:'string',
                makeElem: (val, td, row) => this._makeDescriptionElem(row),
                onEdit: (row, col, e, td) => {
                    //XXX this is really ugly with the hint texts
                    row.bit.description = td.innerText;
                    console.log("GameBit description changed", row, td.innerText);
                    this.btnSave.classList.add('unsaved');
                }
            },
        ]});
    }

    _makeRow(bit) {
        const slot = this.app.saveSlot;
        let hint = '';
        return {
            bit:         bit,
            id:          bit.id,
            table:       bit.table,
            hintId:      bit.hintId,
            hint:        hint,
            size:        bit.size,
            offset:      bit.offset,
            name:        bit.name,
            value:       slot ? slot.gameBits[bit.id] : '',
            description: bit.description,
            notes:       bit.notes,
            hint:        bit.hint,
            objRefs:     bit.objRefs,
        };
    }

    _makeObjRefsElem(row) {
        const items = [];
        for(let objRef of row.objRefs) {
            items.push(E.div('objref',
                E.span('mapId', objRef.map), '.',
                E.span('objNo', objRef.obj), '.',
                E.span('objId', hex(objRef.objId, 8)), '.',
                E.span('param', objRef.param),
            ));
        }
        return E.td('string', CollapseList(...items));
    }

    _makeDescriptionElem(row) {
        const items = [];
        if(row.description) {
            items.push(E.span('description', row.description));
        }
        if(row.hint) items.push(E.span('hintText', row.hint));
        for(let note of row.notes) {
            items.push(E.span('note', note));
        }
        return E.td('string', CollapseList(...items));
    }

    _save() {
        /** Called when Save button is clicked. */
        const xml = document.implementation.createDocument(XML, "gamebits");
        for(let [id, bit] of Object.entries(this.app.game.bits)) {
            xml.documentElement.appendChild(bit.toXml());
        }
        downloadXml(xml, 'gamebits');
        this.btnSave.classList.remove('unsaved');
    }
}
