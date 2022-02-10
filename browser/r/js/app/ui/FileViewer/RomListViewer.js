import { E, clearElement } from "../../../lib/Element.js";
import { hex } from "../../../Util.js";
import Struct from "../../../lib/Struct.js";
import { Vec3f } from "../../../types/common.js";
import Table from "../Table.js";

export const RomList = Struct(
    ['h',   'objDef'],
    ['B',   'length'], //in 4-byte words
    ['B',   'acts0'],
    ['B',   'loadFlags'],
    ['B',   'acts1'],
    ['B',   'bound'],
    ['B',   'cullDist'],
    [Vec3f, 'position'],
    ['i',   'id'],
    //following is object-specific params
);

export class RomListViewer {
    //displays romlist data
    constructor(app, dataView) {
        this.app     = app;
        this.view    = dataView;
        this.element = E.div('romlist');
        this.refresh();
    }

    refresh() {
        this.table = new Table({columns: [
            {displayName:"ID", name:'id', type:'hex', length:8},
            {displayName:"DefNo", name:'objDef', type:'hex', length:4},
            {displayName:"Object", name:'object', type:'str'},
            {displayName:"X", name:'x', type:'int'},
            {displayName:"Y", name:'y', type:'int'},
            {displayName:"Z", name:'z', type:'int'},
            {displayName:"Params", name:'params', type:'str'},
        ]});

        for(let offs=0; offs<this.view.byteLength;) {
            let entry = new RomList(this.view, offs);
            this.table.add(this._makeRow(this.view, offs, entry));
            offs += entry.length * 4;
        }

        clearElement(this.element);
        this.element.append(this.table.element);
    }

    _makeRow(view, offs, entry) {
        const row = {
            id: entry.id,
            objDef: entry.objDef,
            object: "TODO",
            x: Math.round(entry.position.x),
            y: Math.round(entry.position.y),
            z: Math.round(entry.position.z),
        };

        let params = [];
        for(let xOffs=0x18; xOffs<entry.length*4; xOffs += 4) {
            params.push(hex(view.getUint32(xOffs+offs), 8));
        }
        row.params = params.join(' ');
        return row;
    }
}
