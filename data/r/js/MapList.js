import {E} from './Element.js';
import Table from './Table.js';
import {hex, makeList, makeCollapsibleList, makeDescriptionAndNotes} from './Util.js';

export default class MapList {
    /** A page listing the maps in the game.
     */
    constructor(app) {
        this.app = app;
        this._makeTable();
        this.element = this.tbl.element;
    }

    getTitle() { return "Maps" }

    async run() {
        for(let [id, map] of Object.entries(this.app.game.maps)) {
            this.tbl.add(this._makeRow(map));
        }
    }

    _makeTable() {
        const v = this.app.game.version;
        this.tbl = new Table({
            columns: [
                {
                    displayName: "#",
                    title: "Map ID",
                    name: 'id',
                    type: 'hex',
                    length: 2,
                    makeElem: (val, td, row) => E.td('hex',
                        E.a(null, val, {href:`?v=${v}&p=map&romlist=${row.romlistName}&id=${row.id}`}),
                    ),
                }, {
                    displayName: "D",
                    title: "Directory ID",
                    name: 'dirId',
                    type: 'hex',
                    length: 2,
                }, {
                    displayName: "Dir",
                    title: "Directory Name",
                    name: 'dirName',
                    type: 'string',
                    length: 2,
                }, {
                    displayName: "P",
                    title: "Parent Map ID",
                    name: 'parentId',
                    type: 'hex',
                    length: 2,
                    makeElem: (val, td, row) => {
                        td.setAttribute('title', row.parentName);
                        return td;
                    },
                }, {
                    displayName: "T",
                    title: "Map Type",
                    name: 'type',
                    type: 'int',
                }, {
                    displayName: "RomList",
                    title: "Romlist File Name",
                    name: 'romlistName',
                    type: 'string',
                    makeElem: (val, td, row) => E.td('string',
                        E.a(null, val, {href:`?v=${v}&p=map&romlist=${row.romlistName}&id=${row.id}`}),
                    ),
                }, {
                    displayName: "Internal Name",
                    title: "Name from MAPINFO.bin",
                    name: 'internalName',
                    type: 'string',
                }, {
                    displayName: "WX",
                    title: "World X position",
                    name: 'worldX',
                    type: 'int',
                }, {
                    displayName: "WY",
                    title: "World Y position",
                    name: 'worldY',
                    type: 'int',
                }, {
                    displayName: "OX",
                    title: "Origin X",
                    name: 'originX',
                    type: 'int',
                }, {
                    displayName: "OY",
                    title: "Origin Y",
                    name: 'originY',
                    type: 'int',
                }, {
                    displayName: "L0",
                    title: "Linked Map",
                    name: 'link0',
                    type: 'hex',
                    length: 2,
                    makeElem: (val, td, row) => {
                        td.setAttribute('title', row.link0Name);
                        return td;
                    },
                }, {
                    displayName: "L1",
                    title: "Linked Map",
                    name: 'link1',
                    type: 'hex',
                    length: 2,
                    makeElem: (val, td, row) => {
                        td.setAttribute('title', row.link1Name);
                        return td;
                    },
                }, {
                    displayName: "InfOfs",
                    title: "Info Offset",
                    name: 'infoOffset',
                    type: 'hex',
                    length: 6,
                }, {
                    displayName: "BlkTbl",
                    title: "Block Table Offset",
                    name: 'blockTable',
                    type: 'hex',
                    length: 6,
                }, {
                    displayName: "UnkOf2",
                    title: "Unknown Offset 2",
                    name: 'rects1',
                    type: 'hex',
                    length: 6,
                }, {
                    displayName: "UnkOf3",
                    title: "Unknown Offset 3",
                    name: 'rects2',
                    type: 'hex',
                    length: 6,
                }, {
                    displayName: "UnkOf4",
                    title: "Unknown Offset 4",
                    name: 'rects3',
                    type: 'hex',
                    length: 6,
                }, {
                    displayName: "UnkOf5",
                    title: "Unknown Offset 5",
                    name: 'rects4',
                    type: 'hex',
                    length: 6,
                }, {
                    displayName: "UnkOf6",
                    title: "Unknown Offset 6",
                    name: 'romlistOffs',
                    type: 'hex',
                    length: 6,
                }, {
                    displayName: "Unk08",
                    name: 'unk08',
                    type: 'hex',
                    length: 8,
                }, {
                    displayName: "Unk0C",
                    name: 'unk0C',
                    type: 'hex',
                    length: 8,
                }, {
                    displayName: "Unk10",
                    name: 'unk10',
                    type: 'hex',
                    length: 8,
                }, {
                    displayName: "Unk14",
                    name: 'unk14',
                    type: 'hex',
                    length: 8,
                }, {
                    displayName: "Unk18",
                    name: 'unk18',
                    type: 'hex',
                    length: 8,
                }, {
                    displayName: "Uk1C",
                    name: 'unk1C',
                    type: 'hex',
                    length: 4,
                }, {
                    displayName: "Uk1E",
                    name: 'unk1E',
                    type: 'hex',
                    length: 4,
                }, {
                    displayName: "Description",
                    name: 'description',
                    type: 'string',
                    format: (desc, col, row) => makeDescriptionAndNotes(row),
                },
            ]
        });
    }

    _makeRow(map) {
        const getName = (id) => {
            const map = this.app.game.maps[id];
            if(!map) return null;
            return map.romlistName;
        }
        const getNameByDir = (id) => {
            const map = this.app.game.mapsByDir[id];
            if(!map) return null;
            return map.romlistName;
        }

        return {
            id:           map._id,
            cls:          map.unused ? 'unused' : '',
            dirId:        map.dirId,
            type:         map.type,
            param3:       map.param3,
            param4:       map.param4,
            romlistName:  map.romlistName,
            dirName:      map.dirName,
            internalName: map.internalName,
            parentId:     map.parentId,
            parentName:   getNameByDir(map.parentId),
            worldX:       map.worldX,
            worldY:       map.worldY,
            layer:        map.layer,
            link0:        map.link0,
            link1:        map.link1,
            link0Name:    getName(map.link0),
            link1Name:    getName(map.link1),
            width:        map.width,
            height:       map.height,
            originX:      map.originX,
            originY:      map.originY,
            infoOffset:   map.infoOffset,
            blockTable:   map.blockTable,
            rects1:       map.rects1,
            rects2:       map.rects2,
            rects3:       map.rects3,
            rects4:       map.rects4,
            romlistOffs:  map.romlistOffs,
            unk08:        map.unk08,
            unk0C:        map.unk0C,
            unk10:        map.unk10,
            unk14:        map.unk14,
            unk18:        map.unk18,
            unk1C:        map.unk1C,
            unk1E:        map.unk1E,
            description:  map.description,
            notes:        map.notes,
        };
    }
}
