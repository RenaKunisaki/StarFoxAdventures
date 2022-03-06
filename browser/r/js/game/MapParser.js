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
    /** Parses MAPS.bin, MAPINFO.bin, romlists, etc for all maps. */
    constructor(app) {
        this.app = app;
        this.maps = {};
        this._mapsByDirId = {};
    }
    _getMapById(id) {
        let map = this.maps[id];
        if(map == undefined) {
            map = {id:id};
            this.maps[id] = map;
        }
        return map;
    }
    _getMapByDirId(id) {
        let map = this._mapsByDirId[id];
        if(map == undefined) {
            map = {dirId:id};
            this._mapsByDirId[id] = map;
        }
        return map;
    }
    async parse() {
        /** Read map info from game. */
        this.readIdxTable();
        this.readMapNames();
        this.readDirNames();
        this.readParents();
        await this.parseMapsBin();
        this.parseMapInfo();
        return this.maps;
    }
    readMapNames() {
        //romlist file names
        if(this.app.game.version == 'K0') return; //no names
        const dol    = this.app.game.iso.mainDol;
        const file   = new GameFile(dol.getData());
        const aNames = this.app.game.addresses.mapName;
        for(let iMap=0; iMap<aNames.count; iMap++) {
            const map = this._getMapById(iMap);
            file.seek(dol.addrToOffset(aNames.address + (iMap*4)));
            const ptr = file.readU32();
            file.seek(dol.addrToOffset(ptr));
            map.romListName = file.readStr(256);
        }
    }
    readDirNames() {
        //asset dir names
        const dol    = this.app.game.iso.mainDol;
        const file   = new GameFile(dol.getData());
        const aNames = this.app.game.addresses.mapDirNames;
        for(let iMap=0; iMap<aNames.count; iMap++) {
            const map = this._getMapByDirId(iMap);
            file.seek(dol.addrToOffset(aNames.address + (iMap*4)));
            const ptr = file.readU32();
            file.seek(dol.addrToOffset(ptr));
            map.dirName = file.readStr(256);
        }
    }
    readIdxTable() {
        //translates map ID to MAPINFO.BIN index
        const dol    = this.app.game.iso.mainDol;
        const file   = new GameFile(dol.getData());
        const aTable = this.app.game.addresses.mapIdXltnTbl;
        file.seek(dol.addrToOffset(aTable.address));
        for(let iMap=0; iMap<aTable.count; iMap++) {
            let id = file.readS32();
            if(id < 0) break;
            const map = this._getMapById(iMap);
            map.dirId = id;
            this._mapsByDirId[map.dirId] = map;
        }
    }
    readParents() {
        const dol    = this.app.game.iso.mainDol;
        const file   = new GameFile(dol.getData());
        const aTable = this.app.game.addresses.parentMapId;
        file.seek(dol.addrToOffset(aTable.address));
        for(let iMap=0; iMap<aTable.count; iMap++) {
            const map = this._getMapById(iMap);
            let parent = file.readS16();
            //if(parent == -1) parent = null;
            map.parentId = parent;
        }
    }
    parseMapInfo() {
        this.mapInfo = new GameFile(this.app.game.iso.getFile('/MAPINFO.bin'));
        for(let map of Object.values(this.maps)) {
            if(map.id != undefined) {
                let entry = new MapInfoEntry(this.mapInfo,
                    map.id * MapInfoEntry._size);
                map.name    = entry.name;
                map.type    = entry.type;
                map.unk1D   = entry.unk1D;
                map.objType = entry.objType;
            }
        }
    }
    async parseMapsBin() {
        this.mapsBin = new GameFile(this.app.game.iso.getFile('/MAPS.bin'));
        this.mapsTab = new GameFile(this.app.game.iso.getFile('/MAPS.tab'));
        const nMaps  = Math.floor(this.mapsTab.byteLength / MapsTabEntry._size);
        for(let iMap=0; iMap<nMaps; iMap++) {
            await this.app.progress.update({
                subText:   "Parsing maps...",
                numSteps:  nMaps, stepsDone: iMap,
            });
            let tab  = new MapsTabEntry (this.mapsTab, iMap * MapsTabEntry._size);
            let info = new MapsBinEntry0(this.mapsBin, tab.info);
            let map  = this._getMapById(iMap);
            map.sizeX   = info.sizeX;
            map.sizeZ   = info.sizeZ;
            map.originX = info.originX;
            map.originZ = info.originZ;
            map.nBlocks = info.nBlocks;
            map.unk08   = info.unk08;
            map.unk0C   = info.unk0C;
            map.unk1E   = info.unk1E;
            map.blocks  = this._readBlocks(map, tab.blocks);
            this._readRomList(tab.romList, map);
        }
    }
    _readBlocks(map, offset) {
        const result = [];
        let blocks;
        this.mapsBin.seek(offset);
        try {
            blocks = this.mapsBin.readU32Array(map.nBlocks);
        }
        catch(ex) {
            if(ex instanceof RangeError) {
                console.log("Map has invalid blocks offset:", map);
                return [];
            }
            else throw ex;
        }
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
            const data = this.mapsBin.decompress(offset);
            map.romListSize = data.byteLength;
            map.romList     = new RomList(this.app, data);
        }
        else {
            //for other versions only the size is here, used to tell how
            //much memory to allocate. we don't need to check it.
            //XXX we should, though.
            if(!map.romListName) {
                console.error("No romListName for map", map);
                return null;
            }
            const path = `/${map.romListName}.romlist.zlb`;
            let   file = this.app.game.iso.getFile(path);
            let   data;
            if(!file) {
                console.log('File not found:', path);
                //the game will also check MAPS.bin
                data = new this.mapsBin.decompress(offset);
            }
            else data = new GameFile(file);
            map.romListSize = data.byteLength;
            map.romList     = new RomList(this.app, data);
        }
    }
}
