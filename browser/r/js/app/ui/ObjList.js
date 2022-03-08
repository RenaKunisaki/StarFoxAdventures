import Game from "../../game/Game.js";
import { E } from "../../lib/Element.js";
import { assertType, hex } from "../../Util.js";
import Table from "./Table.js";

export default class ObjList {
    /** Displays all object definitions.
     */
    constructor(game) {
        this.game    = assertType(game, Game);
        this.app     = game.app;
        this.element = document.getElementById('tab-objList');
        //this.app.onSaveSlotChanged(slot => this.refresh());
        this.app.onIsoLoaded(iso => this.refresh());
    } //constructor

    refresh() {
        let tbl = this._makeTable();
        for(let obj of this.game.objects) {
            tbl.add(this._makeRow(obj));
        }
        const elem = E.div('objList', tbl.element);
        this.element.replaceWith(elem);
        this.element = elem;
    }

    _makeTable() {
        return new Table({columns: [
            {displayName:"#",    name:'id',   type:'hex', length:4,
                title:"Index in OBJECTS.bin"},
            {displayName:"Idx#", name:'index',type:'hex', length:4,
                title:"Entry in OBJINDEX.bin"},
            {displayName:"Name", name:'name', type:'string', title:"Internal name"},
            {displayName:"Category",  name:'cat',  type:'string'},
            {displayName:"DLL",  name:'dll',  type:'string',
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
            {displayName:"Flags", name:'flags', type:'hex', length:8},
            {displayName:"#P",    name:'nPlayerObjs', type:'int', title:"Player Objs"},
            {displayName:"#M",    name:'nModels', type:'int', title:"Num. Models"},
            {displayName:"#T",    name:'nTextures', type:'int', title:"# Textures"},
            {displayName:"Seqs",  name:'nSeqs', type:'int'},
            {displayName:"Map",   name:'map', type:'hex', length:2,
                makeElem: (val, td, row) => {
                    if(val < 0) td.classList.add('null');
                    return td;
                },
            },
        ]});
    }

    _makeRow(obj) {
        let cat = this.game.objCats[obj.catId];
        if(cat == undefined) cat = `#${obj.catId}`;

        let dll = null;
        if(obj.dll_id >= 0) dll = this.game.dlls[obj.dll_id];

        const row = {
            id:          obj.id,
            index:       obj.index,
            name:        obj.name,
            cat:         cat,
            dll:         dll,
            flags:       obj.flags,
            nModels:     obj.nModels,
            nTextures:   obj.nTextures,
            nPlayerObjs: obj.nPlayerObjs,
            nSeqs:       obj.nSeqs,
            map:         obj.map,
            //XXX more
        };
        return row;
    }
}
