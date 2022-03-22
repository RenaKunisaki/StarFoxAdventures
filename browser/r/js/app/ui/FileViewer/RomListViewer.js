import { E, clearElement } from "../../../lib/Element.js";
import { assertType, hex } from "../../../Util.js";
import Table from "../Table.js";
import RomList from "../../../game/map/RomList.js";
import Game from "../../../game/Game.js";

export class RomListViewer {
    //displays romlist data
    constructor(game, dataView) {
        this.game    = assertType(game, Game);
        this.app     = game.app;
        this.view    = dataView;
        this.element = E.div('romlist');
        this.refresh();
    }

    refresh() {
        this.objIndex = this.game.iso.getFile('/OBJINDEX.bin').getData();
        this.objsTab  = this.game.iso.getFile('/OBJECTS.tab').getData();
        this.objsBin  = this.game.iso.getFile('/OBJECTS.bin').getData();

        this.table = new Table({title:"Romlist Data", columns: [
            {displayName:"ID", name:'id', type:'hex', length:8},
            {displayName:"Obj#", name:'objDef', type:'hex', length:4, title:"Object index"},
            {displayName:"Def#", name:'defNo', type:'hex', length:4, title:"Object defNo"},
            {displayName:"Object", name:'object', type:'str'},
            {displayName:"X", name:'x', type:'int'},
            {displayName:"Y", name:'y', type:'int'},
            {displayName:"Z", name:'z', type:'int'},
            {displayName:"Params", name:'params', type:'str',
                makeElem: (params, td, row) => {
                    if(typeof(params) == 'string') {
                        td.innerText = params;
                        return td;
                    }
                    const ul = E.ul('params');
                    const names = Object.keys(params);
                    names.sort();

                    for(let name of names) {
                        const param = params[name];
                        let title = `[${param.param.type}] `;
                        if(param.param.description) {
                            title += param.param.description;
                        }
                        let disp = param.value;
                        if(disp != null && disp != undefined) {
                            disp = disp.display;
                            if(disp == null || disp == undefined) {
                                disp = param.value.value;
                            }
                        }
                        disp = String(disp);
                        const li = E.li(`param param-${param.param.type}`,
                            E.span('name',  name), ': ',
                            E.span('value', disp),
                            {title:title},
                        );
                        ul.append(li);
                    }
                    clearElement(td).append(ul);
                    return td;
                }
            },
        ]});

        this.romList = new RomList(this.game, this.view);
        for(let entry of this.romList.entries) {
            this.table.add(this._makeRow(entry));
        }

        clearElement(this.element);
        this.element.append(this.table.element);
    }

    _makeRow(entry) {
        let defNo = entry.objDef;
        if(defNo < 0) defNo = -defNo;
        else {
            if(this.game.objIndex[defNo]) {
                defNo = this.game.objIndex[defNo];
            }
            else {
                console.log("Invalid defNo", defNo);
                defNo = 0;
            }
        }

        const obj = this.game.getObject(entry.objDef);
        const row = {
            id:     entry.id,
            objDef: entry.objDef,
            defNo:  defNo,
            object: obj ? obj.name : '?',
            x: Math.round(entry.position.x),
            y: Math.round(entry.position.y),
            z: Math.round(entry.position.z),
        };

        let params = [];
        if(entry.params == null || Object.keys(entry.params).length == 0) {
            for(let xOffs=0x18; xOffs<entry.paramData.byteLength; xOffs += 4) {
                params.push(hex(entry.paramData.getUint32(xOffs), 8));
            }
            row.params = params.join(' ');
        }
        else row.params = entry.params;
        return row;
    }
}
