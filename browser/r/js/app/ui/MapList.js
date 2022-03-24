import { E, clearElement } from "../../lib/Element.js";
import Table from "./Table.js";
import { assertType, hex } from "../../Util.js";
import Game from "../../game/Game.js";

export default class MapList {
    /** Displays list of maps.
     */
    constructor(game) {
        this.game    = assertType(game, Game);
        this.app     = game.app;
        this.element = document.getElementById('tab-mapList');
        this.app.onIsoLoaded(iso => this.refresh());
    }

    refresh() {
        let tbl = this._makeTable();
        for(let [id, map] of Object.entries(this.game.maps)) {
            tbl.add(this._makeRow(map));
        }
        const elem = E.div('mapList', tbl.element);
        clearElement(this.element).append(elem);
    }

    _makeMapElem(val, td, row, isDir=false) {
        if(val == null || val <= 0) {
            td.classList.add('null');
            td.innerText = "none";
            return td;
        }
        const map = isDir ? this.game.mapsByDirId[val]
            : this.game.maps[val];
        if(map) {
            clearElement(td).append(
                E.span('isDir', isDir ? 'D' : ''),
                E.span('id', hex(val,2)),
                E.span('name', map.name),
            );
        }
        else {
            td.classList.add('invalid');
            clearElement(td).append(E.span('id', hex(val,2)));
            td.setAttribute('title', "Invalid");
        }
        return td;
    }

    _makeDirElem(val, td, row) {
        if(row.dirId == 5) {
            td.classList.add('animtest');
        }
        else {
            const dir = this.game.iso.getFile(`/${row.dirName}`);
            if(dir) {
                if(!this.game.iso.getFile(`/${row.dirName}/MODELS.bin`)) {
                    td.classList.add('missing');
                    td.setAttribute('title', "Empty dir");
                }
            }
            else {
                td.classList.add('missing');
                td.setAttribute('title', "Missing dir");
            }
        }
        return td;
    }

    _makeTable() {
        const makeMapElem = (v,t,r) => this._makeMapElem(v,t,r);
        const makeDirElem = (v,t,r) => this._makeDirElem(v,t,r);
        return new Table({title:"Maps", columns: [
            {displayName:"#", name:'id', type:'hex', length:2},
            {displayName:"Name", name:'name', type:'string',
                makeElem: (val, td, row) => {
                    if(!row.isUsed) {
                        td.classList.add('unused');
                        td.setAttribute('title', "Unused");
                    }
                    return td;
                }
            },
            {displayName:"#D", name:'dirId', type:'hex', length:2,
                title:"Dir ID", makeElem: makeDirElem,
            },
            {displayName:"Dir", name:'dirName', type:'string',
                makeElem: makeDirElem,
            },
            {displayName:"Romlist", name:'romListName', type:'string',
                makeElem: (val, td, row) => {
                    const file = this.game.iso.getFile(
                        `/${row.romListName}.romlist.zlb`);
                    if(file) {
                        //heuristic since we don't need to add a bunch of
                        //overhead by actually decompressing and parsing
                        //every romlist just to check if it only contains
                        //one or two items.
                        if(file.size <= 64) td.classList.add('empty');
                        td.addEventListener('click', async e => {
                            await this.app.showFileInNewWindow(file);
                        });
                        td.classList.add('link');
                        td.setAttribute('title', `Size: ${file.size}`);
                    }
                    else {
                        td.classList.add('missing');
                        td.setAttribute('title', "Missing file");
                    }
                    return td;
                }
            },
            {displayName:"RLen", name:'romListSize', type:'int',
                title:"RomList uncompressed size"},
            {displayName:"Parent", name:'parentId', type:'string',
                makeElem: (val, td, row) => makeMapElem(val, td, row, true),
            },
            {displayName:"T",  name:'type', type:'int', title:"Type"},
            {displayName:"WX", name:'worldX', type:'int', title:"World X"},
            {displayName:"WZ", name:'worldZ', type:'int', title:"World Z"},
            {displayName:"SX", name:'sizeX', type:'int', title:"Size X"},
            {displayName:"SZ", name:'sizeZ', type:'int', title:"Size Z"},
            {displayName:"Ly", name:'layer', type:'int', title:"Layer"},
            {displayName:"OX", name:'originX', type:'int', title:"Origin X"},
            {displayName:"OZ", name:'originZ', type:'int', title:"Origin Z"},
            {displayName:"Link0", name:'link0', type:'string', title:"Linked Map",
                makeElem: makeMapElem},
            {displayName:"Link1", name:'link1', type:'string', title:"Linked Map",
                makeElem: makeMapElem},
            {displayName:"#Blk",  name:'nBlocks', type:'int', title:"Number of blocks"},
            {displayName:"Player",name:'objType', type:'string',
                title:"Player object (ignored in final version)",
                makeElem: (val, td, row) => {
                    let res = "default";
                    if(val != 0) {
                        const obj = this.game.getObject(val);
                        res = obj ? obj.name : 'N/A';
                    }
                    td.innerText = `${hex(val,4)} ${res}`;
                    return td;
                },
            },
            {displayName:"unk08", name:'unk08', type:'hex', length:8},
            //XXX unk0C is an array of 4 values
            //also it's always zeros except for two maps that also have
            //negative sizes, so it's probably some padding/unused field
            //{displayName:"unk0C", name:'unk0C', type:'hex', length:4},
            {displayName:"unk1D", name:'unk1D', type:'hex', length:4},
            {displayName:"Flgs",  name:'unk1E', type:'hex', length:4,
                title:"MAPS.bin flags or something"},
            {displayName:"Description", name:'description', type:'string'},
        ]});
    }

    _makeRow(map) {
        return {
            id:              map.id,
            dirId:           map.dirId,
            dirName:         map.dirName,
            romListName:     map.romListName,
            romListSize:     map.romListSize,
            name:            map.name,
            type:            map.type,
            parentId:        map.parentId,
            objType:         map.objType,
            worldX:          map.worldX,
            worldZ:          map.worldZ,
            layer:           map.layer,
            sizeX:           map.sizeX,
            sizeZ:           map.sizeZ,
            originX:         map.originX,
            originZ:         map.originZ,
            link0:           map.links ? map.links[0] : '',
            link1:           map.links ? map.links[1] : '',
            blocks:          map.blocks,
            nBlocks:         map.nBlocks,
            description:     map.description,
            isUsed:          map.isUsed,
            unk08:           map.unk08,
            unk0C:           map.unk0C,
            unk1D:           map.unk1D,
            unk1E:           map.unk1E,
        };
    }
}
