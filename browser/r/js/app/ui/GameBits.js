import { E } from "../../lib/Element.js";
import { hex, Percent } from "../../Util.js";
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
        for(let [id, bit] of Object.entries(this.app.gameBits)) {
            tbl.add(this._makeRow(bit));
        }
        console.log("Bit table", tbl);
        const elem = E.div('saveInfo', tbl.element);
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
                compareFunc: (a, b) => a.nObjRefs - b.nObjRefs,
            },
            {displayName:"Description", name:'description', type:'string',
                //makeElem: (val, td, row) => {
                //    return E.td('string', makeDescriptionAndNotes(row),
                //        E.div('hint', row.hint),
                //    );
                //},
            },
        ]});
    }

    _makeRow(bit) {
        const slot = this.app.saveSlot;
        let hint = '';
        //if(bit.hintId) {
        //    hint = makeCollapsibleList(bit.hint);
        //}
        //let objRefs = [];
        //for(let r of bit.objRefs) {
        //    //to make these link to obj/map we need to re-parse the data
        //    //and include their IDs in the files, since names can be
        //    //ambiguous.
        //    objRefs.push(E.span('objref',
        //        E.span('objid', hex(r.objId,8)),
        //        E.span('mapname',   r.map), //XXX link to the map
        //        E.span('objname',   r.obj), //XXX link to the object
        //        E.span('hex',       r.param),
        //    ));
        //}
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
            //objRefs:     makeCollapsibleList(objRefs),
            //nObjRefs:    bit.objRefs.length,
        };
    }
}
