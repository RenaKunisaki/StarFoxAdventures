import Struct from "../lib/Struct.js"
import RomList from "../types/RomList.js";
import GameFile from "./GameFile.js";

const MapsBinEntry0 = Struct(
    ['h',  'sizeX'],
    ['h',  'sizeZ'],
    ['h',  'originX'],
    ['h',  'originZ'],
    ['I',  'unk08'],
    ['4I', 'unk0C'],
    ['h',  'nBlocks'],
    ['H',  'unk1E'],
);
const MapsTabEntry = Struct(
    ['I',  'info'], // -> MapsBinEntry0
    ['I',  'blocks'], // -> u32
    ['4I', 'rects'],
    ['I',  'romList'], //FACEFEED (header only in final version)
);
const MapInfoEntry = Struct(
    ['28s', 'name'],
    ['B',   'type'],
    ['B',   'unk1D'],
    ['h',   'objType'], //ObjDefEnum
);

export default class MapParser {
    /** Parses MAPS.bin, MAPINFO.bin */
    constructor(app) {
        this.app = app;
        this.maps = {};
    }

    parseMapInfo() {
        this.mapInfo = new GameFile(this.app.game.iso.getFile('/MAPINFO.bin'));
        const nMaps = this.mapInfo.size / MapInfoEntry._size;
        const maps  = [];
        for(let iMap=0; iMap<nMaps; iMap++) {
            let entry = new MapInfoEntry(this.mapInfo, iMap * MapInfoEntry._size);
            maps.push({
                name:    entry.name,
                type:    entry.type,
                unk1D:   entry.unk1D,
                objType: entry.objType,
            });
        }
        return maps;
    }

    parseMapsBin() {
        this.mapsBin = new GameFile(this.app.game.iso.getFile('/MAPS.bin'));
        this.mapsTab = new GameFile(this.app.game.iso.getFile('/MAPS.tab'));
        const nMaps  = this.mapsTab.size / MapsTabEntry._size;
        const maps   = [];
        for(let iMap=0; iMap<nMaps; iMap++) {
            let entry = new MapsTabEntry (this.mapsTab, iMap * MapsTabEntry._size);
            let info  = new MapsBinEntry0(this.mapsBin, entry.info);
            let map   = {
                idx:     iMap,
                sizeX:   info.sizeX,
                sizeZ:   info.sizeZ,
                originX: info.originX,
                originZ: info.originZ,
                nBlocks: info.nBlocks,
                unk08:   info.unk08,
                unk0C:   info.unk0C[0],
                unk10:   info.unk0C[1],
                unk14:   info.unk0C[2],
                unk18:   info.unk0C[3],
                unk1E:   info.unk1E,
            };
            map.blocks  = this._readBlocks(info.blocks, map.nBlocks);
            map.romList = this._readRomList(info.romList, map);
            maps.push(map);
        }
        return maps;
    }

    _readBlocks(offset, count) {
        const result = [];
        this.mapsBin.seek(offset);
        let blocks = this.mapsBin.readU32Array(count);
        for(let blockData of blocks) {
            let block = {
                unk1:  blockData >> 31,
                mod:  (blockData >> 23) & 0x00FF,
                sub:  (blockData >> 17) & 0x003F,
                unk2:  blockData        & 0x01FF,
            };
            if(block.mod == 0xFF) block = null;
            result.push(block);
        }
        return result;
    }

    _readRomList(offset, map) {
        if(this.app.game.version == 'K0') {
            //for kiosk version the romlist is inside MAPS.bin
            return new RomList(this.app, this.mapsBin.decompress(offset));
        }
        else {
            //for other versions only the size is here, used to tell how
            //much memory to allocate. we don't need to check it.
            if(!map.dirName) {
                //XXX this has to be read from the DOL
                console.error("Can't read romlist for map with unknown dir");
                return null;
            }
        }
    }
}
