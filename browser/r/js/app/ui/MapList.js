import { E, clearElement } from "../../lib/Element.js";
import Table from "./Table.js";
import { hex } from "../../Util.js";

export default class MapList {
    /** Displays list of maps.
     */
    constructor(app) {
        this.app = app;
        this.element = document.getElementById('tab-mapList');
        this.app.onIsoLoaded(iso => this.refresh());
    }

    refresh() {
        let tbl = this._makeTable();
        for(let [id, map] of Object.entries(this.app.game.maps)) {
            tbl.add(this._makeRow(map));
        }
        const elem = E.div('mapList', tbl.element);
        this.element.replaceWith(elem);
        this.element = elem;
    }

    _makeTable() {
        const makeMapElem = (val, td, row) => {
            if(val == null || val <= 0) {
                td.classList.add('null');
                td.innerText = "-";
                return td;
            }
            const map = this.app.game.maps[val];
            if(map) {
                clearElement(td).append(
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
        };
        const makeDirElem = (val, td, row) => {
            if(row.dirId == 5) {
                td.classList.add('animtest');
            }
            else {
                const dir = this.app.game.iso.getFile(`/${row.dirName}`);
                if(dir) {
                    if(!this.app.game.iso.getFile(`/${row.dirName}/MODELS.bin`)) {
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
        };
        return new Table({columns: [
            {displayName:"#", name:'id', type:'hex', length:2},
            {displayName:"Name", name:'name', type:'string',
                makeElem: (val, td, row) => {
                    if(!row.used) {
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
                    const file = this.app.game.iso.getFile(
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
                        if(file.size != row.romListCompSize) {
                            td.classList.add('invalid');
                        }
                    }
                    else {
                        td.classList.add('missing');
                        td.setAttribute('title', "Missing file");
                    }
                    return td;
                }
            },
            {displayName:"CLen", name:'romListCompSize', type:'int',
                title:"RomList compressed size"},
            {displayName:"RLen", name:'romListSize', type:'int',
                title:"RomList uncompressed size"},
            {displayName:"Parent", name:'parentId', type:'string',
                //makeElem: makeMapElem,
                //XXX this value is a dir ID, not a map ID
            },
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
            {displayName:"MAPSofs0", name:'mapsBin0Offset', type:'hex', length:8},
            {displayName:"MAPSunk8", name:'mapsBinUnk08', type:'hex', length:2,
                title:"MAPS.bin unknown field 0x8"},
            {displayName:"MAPSunkC", name:'mapsBinUnk0C', type:'hex', length:2,
                title:"MAPS.bin unknown field 0xC"},
            {displayName:"1C", name:'mapsBinUnk1C', type:'hex', length:4,
                title:"MAPS.bin unknown field 0x1C"},
            {displayName:"Flgs", name:'bin0flags', type:'hex', length:4,
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
            name:            map.name,
            type:            map.type,
            parentId:        map.parentId,
            playerObj:       map.playerObj,
            worldX:          map.worldX,
            worldZ:          map.worldZ,
            layer:           map.layer,
            sizeX:           map.sizeX,
            sizeZ:           map.sizeZ,
            originX:         map.originX,
            originZ:         map.originZ,
            link0:           map.links[0],
            link1:           map.links[1],
            blocks:          map.blocks,
            description:     map.description,
            used:            map.used,
            romListSize:     map.romListSize,
            romListCompSize: map.romListCompSize,
            mapsBinUnk08:    map.mapsBinUnk08,
            mapsBinUnk0C:    map.mapsBinUnk0C,
            mapsBinUnk1C:    map.mapsBinUnk1C,
            bin0flags:       map.bin0flags,
            mapsBin0Offset:  map.mapsBin0Offset,
            blocksOffset:    map.blocksOffset,
            rectOffsets:     map.rectOffsets,
            romListHeaderOffset: map.romListHeaderOffset,
        };
    }
}
