import Game from "../../game/Game.js";
import { E } from "../../lib/Element.js";
import { assertType } from "../../Util.js";
import Table from "./Table.js";

export default class DllList {
    /** Displays DLL info.
     */
    constructor(game) {
        this.game = assertType(game, Game);
        this.app  = game.app;
        this.element = document.getElementById('tab-dllList');
        this.app.onIsoLoaded(iso => this.refresh());
    } //constructor

    refresh() {
        let tbl = this._makeTable();
        for(let [id, dll] of Object.entries(this.game.dlls)) {
            tbl.add(this._makeRow(dll));
        }
        const elem = E.div('dllList', tbl.element);
        this.element.replaceWith(elem);
        this.element = elem;
    }

    _makeTable() {
        return new Table({title:"DLLs", columns: [
            {displayName:"#", name:'id', type:'hex', length:4,
                makeElem: (val, td, row) => {
                    if(!row.isValid) td.classList.add('invalid');
                    return td;
                }
            },
            {displayName:"Name",    name:'name',       type:'string'},
            {displayName:"iface",   name:'interface',  type:'string'},
            {displayName:"Address", name:'address',    type:'hex', length:8},
            {displayName:"DOLOfs",  name:'dolOffs',    type:'hex', length:6},
            {displayName:"Constr",  name:'constructor',type:'hex', length:8},
            {displayName:"Destr",   name:'destructor', type:'hex', length:8},
            {displayName:"#Fn",     name:'nFuncs',     type:'int',
                title:"# Functions",
            },
            {displayName:"Description",name:'description',type:'string'},
        ]});
    }

    _makeRow(dll) {
        const row = {
            id:          dll.id,
            name:        dll.name,
            interface:   dll.interface,
            address:     dll.address,
            dolOffs:     dll.dolOffs,
            constructor: dll.functions[0] ? dll.functions[0].address : null,
            destructor:  dll.functions[1] ? dll.functions[1].address : null,
            nFuncs:      dll.functions.length,
            description: dll.description,
            isValid:     dll.isValid,
        };
        return row;
    }
}
