import { E } from "../../lib/Element.js";
import { hex, Percent } from "../../Util.js";
import Table from "./Table.js";

export default class ObjList {
    /** Displays table of ObjList.
     */
    constructor(app) {
        this.app = app;
        this.element = document.getElementById('tab-objList');
        //this.app.onSaveSlotChanged(slot => this.refresh());
        this.app.onIsoLoaded(iso => this.refresh());
    } //constructor

    refresh() {
        let tbl = this._makeTable();
        for(let obj of this.app.gameObjects) {
            tbl.add(this._makeRow(obj));
        }
        const elem = E.div('objList', tbl.element);
        this.element.replaceWith(elem);
        this.element = elem;
    }

    _makeTable() {
        return new Table({columns: [
            {displayName:"#",        name:'id',      type:'hex', length: 4},
            {displayName:"Name",     name:'name',    type:'string'},
            {displayName:"Cat",      name:'cat',     type:'string'},
            {displayName:"DLL",      name:'dll',     type:'string',
                makeElem: (dll, td, row) => {
                    if(!dll) {
                        return E.td('null', "-");
                    }
                    let res = E.td('string',
                        E.span('id hex', hex(dll.id, 4)),
                        E.span('name', dll.name),
                    );
                    if(!dll.isValid) res.classList.add('invalid');
                    return res;
                },
            },
            {displayName:"Flags", name:'flags', type:'hex', length: 8},
        ]});
    }

    _makeRow(obj) {
        let cat = this.app.objCats[obj.catId];
        if(cat == undefined) cat = `#${obj.catId}`;

        let dll = null;
        if(obj.dll_id >= 0) dll = this.app.dlls[obj.dll_id];

        const row = {
            id:          obj.id,
            name:        obj.name,
            cat:         cat,
            dll:         dll,
            flags:       obj.flags,
            //XXX more
        };
        return row;
    }
}
