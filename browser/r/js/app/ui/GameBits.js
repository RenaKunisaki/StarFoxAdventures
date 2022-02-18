import { E } from "../../lib/Element.js";
import { CollapseList, hex } from "../../Util.js";
import Table from "./Table.js";

export default class GameBits {
    /** Displays table of GameBits.
     */
    constructor(app) {
        this.app = app;
        this.element = document.getElementById('tab-gameBits');
        this.app.onSaveSlotChanged(slot => this.refresh());
        this.app.onIsoLoaded(iso => this.refresh());
    } //constructor

    refresh() {
        let tbl = this._makeTable();
        for(let [id, bit] of Object.entries(this.app.game.bits)) {
            tbl.add(this._makeRow(bit));
        }
        console.log("Bit table", tbl);
        const elem = E.div('gameBits', tbl.element);
        this.element.replaceWith(elem);
        this.element = elem;
    }

    _makeTable() {
        return new Table({columns: [
            {displayName:"#", name:'id',    type:'hex', length: 4},
            {displayName:"T", name:'table', type:'int',
                makeElem: (val, td, bit) => {
                    td.classList.add(`bitTable${bit.table}`);
                    return td;
                }
            },
            {displayName:"H",        name:'hintId',  type:'hex', length: 2},
            {displayName:"Size",     name:'size',    type:'int'},
            {displayName:"Offs",     name:'offset',  type:'hex', length: 4},
            {displayName:"Name",     name:'name',    type:'string'},
            {displayName:"Value",    name:'value',   type:'string', classes:'int'},
            {displayName:"Obj Refs", name:'objRefs', type:'string',
                compareFunc: (a, b) => {
                    let ra = a.objRefs[0];
                    let rb = b.objRefs[0];
                    ra = ra ? `${ra.map} ${ra.objNo} ${ra.param} ${ra.objId}` : '';
                    rb = rb ? `${rb.map} ${rb.objNo} ${rb.param} ${rb.objId}` : '';
                    if(ra > rb) return  1;
                    if(ra < rb) return -1;
                    return 0;
                },
                makeElem: (val, td, row) => {
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
                },
            },
            {displayName:"Description", name:'description', type:'string',
                makeElem: (val, td, row) => {
                    const items = [];
                    if(row.description) {
                        items.push(E.span('description', row.description));
                    }
                    for(let hint of row.hint) {
                        items.push(E.span('hintText', hint));
                    }
                    for(let note of row.notes) {
                        items.push(E.span('note', note));
                    }
                    return E.td('string', CollapseList(...items));
                },
            },
        ]});
    }

    _makeRow(bit) {
        const slot = this.app.saveSlot;
        let hint = '';
        return {
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
}
