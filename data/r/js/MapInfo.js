import {E} from './Element.js';
import Table from './Table.js';
import {hex, toHexString, hexStringToData, makeList, makeCollapsibleList, makeDescriptionAndNotes} from './Util.js';

export default class MapInfo {
    /** A page showing details of a map.
     */
    constructor(app) {
        this.app = app;
        this.element = E.div();
    }

    getTitle() {
        const name = this.map ? this.map.internalName : this.romlistName;
        return `${name} - Map Info`;
    }

    async run() {
        const id = this.app.params.get('id');
        this.romlistName = this.app.params.get('romlist');
        this._makeRomListTable();

        this.romList = await this.app.game.getRomList(id, this.romlistName);
        for(let item of this.romList.items) {
            this.tblRomList.add(this._makeRomListRow(item));
        }

        if(id != undefined && id != null && id != 'null' && this.app.game.maps) {
            this.map = this.app.game.maps[id];
            await this.map.readMapsBin();
            this._buildInfo();
            this._buildMapGrid();
        }
        else {
            this.map = null;
            this.tblMapInfo = '';
            this.tblMapGrid = '';
        }

        this.element.append(
            this.tblMapInfo,
            this.tblMapGrid,
            E.div('box', this.tblRomList.element),
        );
    }

    _makeRomListTable() {
        const v = this.app.game.version;
        this.tblRomList = new Table({
            title: "RomList",
            columns: [
                {
                    displayName: "#",
                    title: "Index",
                    name: 'idx',
                    type: 'hex',
                    length: 4,
                }, {
                    displayName: "Offset",
                    name: 'offset',
                    type: 'hex',
                    length: 6,
                }, {
                    displayName: "UniqID",
                    title: "Object unique ID",
                    name: 'id',
                    type: 'hex',
                    length: 6,
                }, {
                    displayName: "Def#",
                    title: "Object def ID",
                    name: 'type',
                    type: 'hex',
                    length: 4,
                }, {
                    displayName: "Real",
                    title: "Real object def ID from OBJINDEX.bin",
                    name: 'realId',
                    type: 'hex',
                    length: 4,
                }, {
                    displayName: "Name",
                    name: 'name',
                    type: 'string',
                    format: (val, col, row) => E.a('objlink', val, {
                        href: `?v=${v}&p=obj&id=${row.realId}`,
                    }),
                }, {
                    displayName: "Acts",
                    name: 'acts',
                    type: 'string',
                }, {
                    displayName: "Bd",
                    title: "Bound",
                    name: 'bound',
                    type: 'hex',
                    length: 2,
                }, {
                    displayName: "Flags",
                    title: "L: isLevelObject\n"+
                        "M: isManualLoad\n"+
                        "P: OnlyLoadIfNearPlayer\n"+
                        "C: AllocatedByCode\n"+
                        "O: loadForOtherMap\n"+
                        "B: isBlockObject\n"+
                        "4: unknown 0x40\n"+
                        "8: unknown 0x80",
                    name: 'loadFlags',
                    type: 'string',
                }, {
                    displayName: "07",
                    title: "ObjDef unknown 07",
                    name: 'unk7',
                    type: 'hex',
                    length: 2,
                }, {
                    displayName: "X",
                    name: 'x',
                    type: 'float',
                    decimals: 2,
                }, {
                    displayName: "Y",
                    name: 'y',
                    type: 'float',
                    decimals: 2,
                }, {
                    displayName: "Z",
                    name: 'z',
                    type: 'float',
                    decimals: 2,
                }, {
                    displayName: "Params",
                    name: 'params',
                    type: 'string',
                    //format: (val, col, row) => val.join(' '),
                },
            ]});
    }

    _makeRomListRow(item) {
        return {
            idx:       item.idx,
            offset:    item.offset,
            realId:    item.realId,
            name:      item.name,
            acts:      item.acts,
            loadFlags: item.loadFlags,
            type:      item.objDef.type,
            id:        item.objDef.id,
            x:         item.objDef.x,
            y:         item.objDef.y,
            z:         item.objDef.z,
            bound:     item.objDef.bound,
            unk7:      item.objDef.unk7,
            //params:    item.objDef.params,
            params:    this._makeParamsTable(item),
        };
    }

    _makeParamsTable(entry) {
        /** Make parameters table for romlist entry.
         */
        const v = this.app.game.version;
        const obj = this.app.game.objects[entry.realId];
        const data = hexStringToData(entry.objDef.params.join(''));
        const result = E.table('objparams',
            E.tr(
                E.th(null, 'Offs'),
                E.th(null, 'Name'),
                E.th(null, 'Type'),
                E.th(null, 'Value'),
            ),
        );
        if(!data.buffer) return "-";

        //XXX this should probably be moved.
        const view = new DataView(data.buffer);
        //console.log("data", data, view);
        let prevOffs = 0;
        for(let param of obj.params) {
            const offs = param.offset - 0x18;
            //console.log("offs", hex(offs));
            if(offs > prevOffs) {
                result.append(E.tr(
                    E.td('hex', hex(prevOffs+0x18, 4)),
                    E.td('str', ''),
                    E.td('str', ''),
                    E.td('hex', toHexString(data.slice(prevOffs, offs))),
                ));
            }
            let val, size=0;
            switch(param.type) {
                case 's8': case 'int8_t': case 'char': case 'bool':
                    size = 1;
                    val  = view.getInt8(offs).toString();
                    break;

                case 'u8': case 'uint8_t': case 'byte': case 'undefined': case 'undefined1': case 'unk8':
                    size = 1;
                    val  = hex(view.getUint8(offs), 2);
                    break;

                case 's16': case 'int16_t': case 'short':
                    size = 2;
                    val  = view.getInt16(offs).toString();
                    break;

                case 'u16': case 'uint8_t': case 'ushort': case 'undefined2': case 'unk16':
                    size = 2;
                    val  = hex(view.getUint16(offs), 4);
                    break;

                case 's32': case 'int32_t': case 'int': case 'word':
                    size = 4;
                    val  = view.getInt32(offs).toString();
                    break;

                case 'u32': case 'uint32_t': case 'uint': case 'ObjId': case 'ObjectId':
                case 'undefined4': case 'unk32':
                    size = 4;
                    val  = hex(view.getUint32(offs), 4);
                    break;

                case 'float': {
                    size = 4;
                    const h = view.getUint32(offs);
                    val = `${view.getFloat32(offs)} (0x${hex(h,8)})`;
                    break;
                }

                case 'GameBit':
                case 'GameBit16': {
                    size = 2;
                    const bit = view.getUint16(offs);
                    val = E.a('gamebitlink', hex(bit,4), {href:`?v=${v}&p=bit&id=${bit & 0x7FFF}`});
                    break;
                }

                case 'GameTextId16': case 'GameTextId': {
                    size = 2;
                    const id   = view.getUint16(offs);
                    const text = this.app.game.texts[id];
                    if(text) val = `[0x${hex(id,4)}] ${text.phrases[0]}`;
                    else val = `[NOT FOUND: 0x${hex(id,4)}]`;
                    break;
                }
                case 'GameTextId32': {
                    size = 4;
                    const id   = view.getUint32(offs);
                    const text = this.app.game.texts[id];
                    if(text) val = `[0x${hex(id,4)}] ${text.phrases[0]}`;
                    else val = `[NOT FOUND: 0x${hex(id,4)}]`;
                    break;
                }

                case 'MapDirIdx8': {
                    size = 1;
                    const id = view.getUint8(offs);
                    const map = this.app.game.mapsByDir[id];
                    const rid = map ? map.id : 0;
                    val = E.a('maplink', hex(id,2), {href:`?v=${v}&p=map&id=${rid}`});
                    break;
                }

                case 'vec3f': case 'Vec3f': {
                    size = 12;
                    const x = view.getFloat32(offs);
                    const y = view.getFloat32(offs+4);
                    const z = view.getFloat32(offs+8);
                    val = `${x}, ${y}, ${z}`;
                    break;
                }

                case 'vec3s': case 'Vec3s': {
                    size = 6;
                    const x = view.getInt16(offs);
                    const y = view.getInt16(offs+2);
                    const z = view.getInt16(offs+4);
                    val = `${x}, ${y}, ${z}`;
                    break;
                }

                case 'WarpEnum8': { //XXX warptab
                    size = 1;
                    const id = view.getUint8(offs);
                    val = hex(id, 2);
                    break;
                }

                default: {
                    console.warn("Unrecognized param type", param);
                    val = '?';
                }
            }
            result.append(E.tr(
                E.td('hex', hex(param.offset, 4)),
                E.td('str', param.name == null ? '' : param.name),
                E.td('str', param.type),
                E.td('str', val),
            ));
            prevOffs = offs + size;
        }

        let pLen = entry.objDef.size * 4;
        if(obj.paramLength != null && obj.paramLength != pLen) {
            console.warn(`Object paramLength=0x${hex(obj.paramLength)} but romlist entry is 0x${hex(pLen)}`);
        }
        if(pLen - 0x18 > prevOffs) {
            result.append(E.tr(
                E.td('hex', hex(prevOffs+0x18, 4)),
                E.td('str', ''),
                E.td('str', ''),
                E.td('hex', toHexString(data.slice(prevOffs, pLen - 0x18))),
            ));
        }
        return result;
    }

    _buildInfo() {
        const map = this.map;
        let parent = this.app.game.mapsByDir[this.map.parentId];
        parent = parent ? parent.internalName : '-';
        let link0 = this.app.game.maps[this.map.link0];
        link0 = link0 ? link0.internalName : '-';
        let link1 = this.app.game.maps[this.map.link1];
        link1 = link1 ? link1.internalName : '-';

        const tbl = E.table('mapinfo',
            E.tr(E.th(null, "ID"),
                E.td('hex', hex(map.id,2))),
            E.tr(E.th(null, "Internal Name"),
                E.td('string', map.internalName)),
            E.tr(E.th(null, "Romlist"),
                E.td('string', map.romlistName)),
            E.tr(E.th(null, "Dir"),
                E.td('string', `${hex(map.dirId,2)} ${map.dirName}`)),
            E.tr(E.th(null, "Parent Dir"),
                E.td('string', `${hex(map.parentId,2)} ${parent}`)),
            E.tr(E.th(null, "Linked", {rowspan:2}),
                E.td('string', `${hex(map.link0,2)} ${link0}`)),
            E.tr(
                E.td('string', `${hex(map.link1,2)} ${link1}`)),
            E.tr(E.th(null, "Type"),
                E.td('hex', map.type)),
            E.tr(E.th(null, "Unk 3,4"),
                E.td('hex', `${hex(map.param3,8)} ${hex(map.param4,8)}`)),
            E.tr(E.th(null, "World Pos"),
                E.td('coords', `${map.worldX}, ${map.worldY}, layer ${map.layer}`)),
            E.tr(E.th(null, "Origin"),
                E.td('coords', `${map.originX}, ${map.originY}`)),
            E.tr(E.th(null, "Size"),
                E.td('coords', `${map.width}, ${map.height}`)),
            E.tr(E.th(null, "Info Offset"),
                E.td('hex', `${hex(map.infoOffset, 6)}`)),
            E.tr(E.th(null, "Block Table"),
                E.td('hex', `${hex(map.blockTable, 6)}`)),
            E.tr(E.th(null, "Romlist Offs"),
                E.td('hex', `${hex(map.romlistOffs, 6)}`)),
            E.tr(E.th(null, "Romlist Expect Size"),
                E.td('hex', `${hex(map.romlistSize, 6)}`)),
            E.tr(E.th(null, "Romlist Real Size"),
                E.td('hex', `${hex(this.romList.byteLength, 6)}`)),
            E.tr(E.th(null, "Rect Offsets"),
                E.td('hex', `${hex(map.rects1, 6)} ${hex(map.rects2, 6)} ${hex(map.rects3, 6)} ${hex(map.rects4, 6)}`)),
            E.tr(E.th(null, "Unknown"),
                E.td('hex', `${hex(map.unk08, 8)} ${hex(map.unk0C, 8)} ${hex(map.unk10, 8)} ${hex(map.unk14, 8)} ${hex(map.unk18, 8)} ${hex(map.unk1C, 4)} ${hex(map.unk1E, 4)}`)),
            E.tr(E.th(null, "Unused"),
                E.td('string', map.unused ? "Yes" : "No")),
            E.tr(E.th(null, "Description"),
                E.td('string', makeDescriptionAndNotes(map))),

        );
        this.tblMapInfo = E.div('box', E.h2(null, "Map Info"), tbl);
    }

    _buildMapGrid() {
        //not using Table class because this isn't row/column data.
        this.tblMapGrid  = E.table('mapgrid');
        const map        = this.map;
        const w          = map.width;
        const h          = map.height;
        const originX    = map.originX;
        const originY    = map.originY;
        const worldX     = map.worldX;
        const worldY     = map.worldY;
        //const infoOffset = map.infoOffset;
        //const blockTable = map.blockTable;
        //const unkOffs6   = map.unkOffs6; //romlist header

        //build the header row
        let tr = E.tr(E.th());
        for(let x=0; x<w; x++) tr.append(E.th(null, `${x} / ${x+worldX}`));
        tr.append(E.th());
        this.tblMapGrid.append(tr);

        //build the grid
        let i=0;
        for(let y=0; y<h; y++) {
            let tr = E.tr(E.th(null, `${y} / ${y+worldY}`));
            for(let x=0; x<w; x++) {
                let td = E.td();

                let block = map.getBlockByPos(x, y);
                if(block) {
                    let mod  = block.mod;
                    let sub  = block.sub;
                    let unk1 = block.unk1;
                    let unk2 = block.unk2;
                    let rIdx = i*2;
                    let r11 = map.rects[0][rIdx], r12 = map.rects[0][rIdx+1];
                    let r21 = map.rects[1][rIdx], r22 = map.rects[1][rIdx+1];
                    let r31 = map.rects[2][rIdx], r32 = map.rects[2][rIdx+1];
                    let r41 = map.rects[3][rIdx], r42 = map.rects[3][rIdx+1];
                    td.append(
                        E.div('line', `${mod}.${sub} ${unk1} ${unk2}`),
                        E.div('line', (r11&&r12) ? `${r11.x1},${r11.y1}~${r11.x2},${r11.y2}; ${r12.x1},${r12.y1}~${r12.x2},${r12.y2}` : '-'),
                        E.div('line', (r21&&r22) ? `${r21.x1},${r21.y1}~${r21.x2},${r21.y2}; ${r22.x1},${r22.y1}~${r22.x2},${r22.y2}` : '-'),
                        E.div('line', (r31&&r32) ? `${r31.x1},${r31.y1}~${r31.x2},${r31.y2}; ${r32.x1},${r32.y1}~${r32.x2},${r32.y2}` : '-'),
                        E.div('line', (r41&&r42) ? `${r41.x1},${r41.y1}~${r41.x2},${r41.y2}; ${r42.x1},${r42.y1}~${r42.x2},${r42.y2}` : '-'),
                    );
                }
                else td.classList.add('empty');
                if(x==originX && y==originY) {
                    td.classList.add('origin');
                }

                tr.append(td);
                i++;
            }
            tr.append(E.th(null, `${y} / ${y+worldY}`));
            this.tblMapGrid.append(tr);
        }

        tr = E.tr(E.th());
        for(let x=0; x<w; x++) tr.append(E.th(null, `${x} / ${x+worldX}`));
        tr.append(E.th());
        this.tblMapGrid.append(tr);

        this.tblMapGrid = E.div('box', E.h2(null, "Map Grid"), this.tblMapGrid);
    }
}
