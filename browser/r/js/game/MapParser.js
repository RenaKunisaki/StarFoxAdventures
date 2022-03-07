import Struct from "../lib/Struct.js"
import RomList from "../types/RomList.js";
import GameFile from "./GameFile.js";
import { getAttr, getXml, int } from "../Util.js";

export const MapsBinEntry0 = Struct(
    ['h',  'sizeX'],
    ['h',  'sizeZ'],
    ['h',  'originX'],
    ['h',  'originZ'],
    ['I',  'unk08'],
    ['4I', 'unk0C'], //rect related? 4 of these, 4 of those...
    ['h',  'nBlocks'],
    ['H',  'unk1E'], //flags?
);
export const MapsTabEntry = Struct(
    ['I',  'info'], // -> MapsBinEntry0
    ['I',  'blocks'], // -> u32
    ['4I', 'rects'],
    ['I',  'romList'], //-> FACEFEED
);
//in old versions, romList pointed to the actual list; in final it points
//to a FACEFEED header with no data following, which is only used to tell the
//size needed to allocate for the list, which is in a separate file.
//(it will still use the data here if there is no separate file.)
//XXX check for any maps that have old data here.

export const MapInfoEntry = Struct(
    ['28s', 'name'],
    ['B',   'type'],
    ['B',   'unk1D'],   //always 6
    ['h',   'objType'], //ObjDefEnum for player (not used in final)
);
const MapGridItem = Struct(
    ['h',  'x'],     //global X coord
    ['h',  'z'],     //global Z coord
    ['h',  'layer'], //layer (-2 to 2)
    ['h',  'mapId'], //map ID (if <0, marks end of list)
    ['2h', 'link'],  //linked map IDs
);


export class Block {
    /** One block of a map.
     */
    constructor(app, map, x, z, val) {
        this.app  =  app;
        this.map  =  map;
        this.x    =  x;
        this.z    =  z;
        this.unk1 =  val >> 31;
        this.mod  = (val >> 23) & 0x00FF;
        this.sub  = (val >> 17) & 0x003F;
        this.unk2 =  val        & 0x01FF;
    }
}

export class Map {
    /** A map in the game.
     *  @note Map IDs can be confusing because the game refers to them by
     *   different IDs in different contexts:
     *   - id: index into MAPINFO.bin
     *   - dirId: index into directory name list in DOL
     *   Also, there are some files left on the disc from deleted maps;
     *   we assign these negative IDs if they don't have an ID defined.
     *   Also, not all maps have all information; for example, there are
     *   several entries in MAPINFO.bin that have a corresponding romlist
     *   file but no asset directory.
     */
    constructor(app, params={}) {
        this.app         = app;
        this.id          = params.id;          //the map ID
        this.dirId       = params.dirId;       //index into list of map dirs
        this.dirName     = params.dirName;     //name of asset dir
        this.romListName = params.romListName; //name of romlist file
        this.romListSize = params.romListSize; //size to allocate for romlist
        this.romList     = params.romList;     //the RomList instance
        this.name        = params.name;        //internal name from MAPINFO.bin
        this.type        = params.type;        //map type from MAPINFO.bin
        this.parentId    = params.parentId;    //parent map ID
        this.links       = params.links;       //linked map IDs
        this.objType     = params.objType;     //player object type (not used in final)
        this.worldX      = params.worldX;      //coords on global map grid
        this.worldZ      = params.worldZ;
        this.layer       = params.layer;       //which global map grid
        this.sizeX       = params.sizeX;       //in blocks
        this.sizeZ       = params.sizeZ;
        this.originX     = params.originX;     //which block is the origin
        this.originZ     = params.originZ;
        this.blocks      = params.blocks;      //list of blocks
        this.unk08       = params.unk08;
        this.unk0C       = params.unk0C;
        this.unk1D       = params.unk1D;
        this.unk1E       = params.unk1E;
        //following are added manually, not present in the game data
        this.description = params.description; //description
        this.isUsed      = params.isUsed;      //is used in game?
    }

    getBlock(x, z) {
        if(this.blocks == undefined) return null;
        let idx = (z * this.sizeX) + x;
        return this.blocks[idx];
    }

    addBlock(block) {
        if(this.blocks == undefined) this.blocks = [];
        this.blocks.push(block);
    }
}

export class MapParser {
    /** Parses MAPS.bin, MAPINFO.bin, romlists, etc for all maps. */
    constructor(game) {
        this.app           = game.app;
        this.game          = game;
        this.game.maps     = {};
        this.game.mapDirs  = {}; //name => map
        this._mapsByDirId  = {};
        this._usedRomLists = {};
    }
    _getMapById(id) {
        let map = this.game.maps[id];
        if(map == undefined) {
            map = new Map(this.app, {id:id});
            this.game.maps[id] = map;
        }
        return map;
    }
    _getMapByDirId(id) {
        let map = this._mapsByDirId[id];
        if(map == undefined) {
            map = new Map(this.app, {dirId:id});
            this._mapsByDirId[id] = map;
        }
        return map;
    }
    async parse() {
        /** Read map info from game. */
        this.app.progress.show({
            taskText: "Reading maps",
            subText:  "Parsing DOL...",
            numSteps: 1, stepsDone: 0,
        });
        this.readIdxTable();
        this.readMapNames();
        this.readParents();
        this.readDirNames();
        await this.parseMapsBin();
        await this.parseMapInfo();
        await this.parseGlobalMap();
        await this.findStrayRomLists();
        await this.parseMapsXml();

        //ensure all maps are in game.maps and game.mapDirs.
        for(let map of Object.values(this._mapsByDirId)) {
            this.game.maps[map.id] = map;
        }
        for(let map of Object.values(this.game.maps)) {
            map.dirName = this.game.mapDirs[map.dirId];
            this.game.mapsByDirId[map.dirId] = map;
        }
        return this.game.maps;
    }
    readMapNames() {
        //romlist file names
        //Kiosk version doesn't use separate romlist files,
        //so doesn't have this table.
        if(this.app.game.version == 'K0') return;
        const dol    = this.app.game.iso.mainDol;
        const file   = new GameFile(dol.getData());
        const aNames = this.app.game.addresses.mapName;
        for(let iMap=0; iMap<aNames.count; iMap++) {
            const map = this._getMapById(iMap);
            file.seek(dol.addrToOffset(aNames.address + (iMap*4)));
            const ptr = file.readU32();
            file.seek(dol.addrToOffset(ptr));
            map.romListName = file.readStr(256);
            this._usedRomLists[map.romListName] = true;
        }
    }
    readDirNames() {
        //asset dir names
        //this table contains several duplicate entries, which is why we need
        //to track both the dir name and the dir ID (which is just the index
        //into this table).
        const dol    = this.game.iso.mainDol;
        const file   = new GameFile(dol.getData());
        const aNames = this.game.addresses.mapDirNames;
        this.game.mapDirs = [];
        for(let iDir=0; iDir<aNames.count; iDir++) {
            file.seek(dol.addrToOffset(aNames.address + (iDir*4)));
            const ptr = file.readU32();
            file.seek(dol.addrToOffset(ptr));
            this.game.mapDirs.push(file.readStr(256));
        }
    }
    readIdxTable() {
        //translates map dir ID to map ID (MAPINFO.BIN index)
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
        //each map has a parent map ID.
        //if it's not -1, it's the dir ID of another map that should also be
        //loaded alongside this one.
        //this table is a mapping of dir ID => parent dir ID.
        const dol    = this.app.game.iso.mainDol;
        const file   = new GameFile(dol.getData());
        const aTable = this.app.game.addresses.parentMapId;
        file.seek(dol.addrToOffset(aTable.address));
        for(let iMap=0; iMap<aTable.count; iMap++) {
            const map = this._getMapByDirId(iMap);
            let parent = file.readS16();
            //if(parent == -1) parent = null;
            map.parentId = parent;
        }
    }
    async parseMapInfo() {
        await this.app.progress.update({
            subText:  "Parsing MAPINFO.bin...",
            numSteps: 1, stepsDone: 0,
        });
        this.mapInfo = new GameFile(this.game.iso.getFile('/MAPINFO.bin'));
        for(let map of Object.values(this.game.maps)) {
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
    async parseGlobalMap() {
        await this.app.progress.update({
            subText:  "Parsing globalma.bin...",
            numSteps: 1, stepsDone: 0,
        });
        this.game.mapGrid = {}; //layer => array
        const globalMap = app.game.iso.getFile('/globalma.bin').getData();
        for(let i=0; ; i++) {
            let entry = new MapGridItem(globalMap, i * MapGridItem._size);
            if(entry.mapId < 0) break;
            this._parseGlobalMapEntry(entry);
        }
    }
    _parseGlobalMapEntry(entry) {
        const layers = this.game.mapGrid;
        let map = this._getMapById(entry.mapId);
        if(layers[entry.layer] == undefined) layers[entry.layer] = [];
        let layer = layers[entry.layer];
        if(layer[entry.x] == undefined) layer[entry.x] = [];
        map.links = entry.link;
        map.minX  =  999999; map.minZ =  999999;
        map.maxX  = -999999; map.maxZ = -999999;
        for(let x=0; x<map.sizeX; x++) {
            for(let z=0; z<map.sizeZ; z++) {
                let cx = (x + entry.x) - map.originX;
                let cz = (z + entry.z) - map.originZ;
                let iBlock = (z * map.sizeX) + x;
                if(layer[cx] == undefined) layer[cx] = [];

                let relX = x - map.originX;
                let relZ = z - map.originZ;

                //maps can overlap
                if(map.blocks[iBlock] != null || !(layer[cx][cz])) {
                    map.worldX = cx;
                    map.worldZ = cz;
                    map.layer  = entry.layer;
                    layer[cx][cz] = {
                        worldX: cx,
                        worldZ: cz,
                        layer:  entry.layer,
                        mapId:  entry.mapId,
                        link:   entry.link,
                        map:    map,
                        relX:   x - map.originX,
                        relZ:   z - map.originZ,
                        block:  map.blocks[iBlock],
                        isOrigin: x == map.originX && z == map.originZ,
                    };
                    if(relX > map.maxX) map.maxX = relX;
                    if(relZ > map.maxZ) map.maxZ = relZ;
                    if(relX < map.minX) map.minX = relX;
                    if(relZ < map.minZ) map.minZ = relZ;
                }
            }
        }
    }
    async parseMapsBin() {
        this.mapsBin = new GameFile(this.app.game.iso.getFile('/MAPS.bin'));
        this.mapsTab = new GameFile(this.app.game.iso.getFile('/MAPS.tab'));
        const nMaps  = Math.floor(this.mapsTab.byteLength / MapsTabEntry._size);
        for(let iMap=0; iMap<nMaps; iMap++) {
            await this.app.progress.update({
                subText:   "Parsing MAPS.bin...",
                numSteps:  nMaps, stepsDone: iMap,
            });
            let tab  = new MapsTabEntry (this.mapsTab, iMap * MapsTabEntry._size);
            let info = new MapsBinEntry0(this.mapsBin, tab.info);
            let map  = this._getMapById(iMap);
            map.sizeX   = info.sizeX;
            map.sizeZ   = info.sizeZ;
            map.originX = info.originX;
            map.originZ = info.originZ;
            map.nBlocks = info.nBlocks; //maybe memory for block data?
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
            blocks = this.mapsBin.readU32Array(map.sizeX * map.sizeZ);
        }
        catch(ex) {
            if(ex instanceof RangeError) {
                console.log("Map has invalid blocks offset:", map);
                return [];
            }
            else throw ex;
        }
        let x = 0, z = 0;
        for(let blockData of blocks) {
            let block = new Block(this.app, map, x, z, blockData);
            if(block.mod == 0xFF) block = null;
            result.push(block);
            x++;
            if(x >= map.sizeX) { x=0; z++ }
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
                //the game will also check MAPS.bin.
                //XXX check if any maps have old romlists here.
                data = new this.mapsBin.decompress(offset);
            }
            else data = new GameFile(file);
            map.romListSize = data.byteLength;
            map.romList     = new RomList(this.app, data);
        }
    }
    async findStrayRomLists() {
        //find romlist files not referenced by maps
        const iso = this.app.game.iso;
        let iFile = 0;
        let nextId = -1;
        for(let file of iso.files) {
            iFile++;
            if(file.name.endsWith('.romlist.zlb')) {
                await this.app.progress.update({
                    subText:`Parsing ${file.name}...`,
                    numSteps: iso.files.length,
                    stepsDone: iFile,
                });
                let name = file.name.split('.')[0];
                if(!this._usedRomLists[name]) {
                    const map = new Map(this.app);
                    map.romListName = name;
                    this.game.maps[nextId--] = map;
                    this._usedRomLists[name] = true;
                }
            }
        }
    }
    async parseMapsXml() {
        //get maps.xml for additional info
        await this.app.progress.update({
            subText:"Downloading maps.xml...",
            numSteps: 1, stepsDone: 0,
        });
        const xml = await getXml(`data/${this.app.game.version}/maps.xml`);
        if(!xml) return;

        for(let elem of xml.getElementsByTagName('map')) {
            let map;
            let id = int(getAttr(elem, 'id'));
            if(id == undefined) {
                id = int(getAttr(elem, 'dirid'));
                if(id == undefined) continue;
                map = this._getMapByDirId(id);
            }
            else map = this._getMapById(id);
            if(!map) continue;

            let desc = elem.getElementsByTagName('description');
            if(desc && desc[0]) map.description = desc[0].textContent;

            map.isUsed = getAttr(elem, 'used') != '0';
            if(getAttr(elem, 'unused')) map.isUsed = false; //XXX old data
        }
    }
}
