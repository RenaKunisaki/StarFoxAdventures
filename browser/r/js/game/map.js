import { hex, int } from "../Util.js";
import Struct from "../lib/Struct.js";

const MapInfoEntry = Struct(
    ['28s', 'name'],
    ['B',   'type'],
    ['B',   'unk1D'],   //always 6
    ['h',   'objType'], //not used in final; ObjDefId to use as player
);
const MapsTabEntry = Struct(
    ['I',  'info'], // -> MapsBinEntry0
    ['I',  'blocks'],
    ['4I', 'rects'],
    ['I',  'romList'], // -> FACEFEED header
);
//in old versions, romList pointed to the actual list; in final it points
//to a FACEFEED header with no data following, which is only used to tell the
//size needed to allocate for the list, which is in a separate file.

const MapsBinEntry0 = Struct(
    ['h',  'sizeX'],
    ['h',  'sizeZ'],
    ['h',  'originX'],
    ['h',  'originZ'],
    ['I',  'unk08'],
    ['4I', 'unk0C'], //rect related? 4 of these, 4 of those...
    ['h',  'unk1C'],
    ['H',  'flags'], //unsure
);

class Block {
    /** One block of a map.
     */
    constructor(app, map, elem, x=null, z=null) {
        this.app  = app;
        this.map  = map;
        this.mod  = null;
        this.sub  = null;
        this.x    = x;
        this.z    = z;
        this.unk1 = null;
        this.unk2 = null;

        if(elem != undefined && elem != null) {
            if(typeof elem == 'number') { //raw value from MAPS.bin
                this.mod  = (elem >> 23) & 0xFF;
                this.sub  = (elem >> 17) & 0x3F;
                this.unk1 = elem >> 31;
                this.unk2 = elem >> 0x1FF;
            }
            else { //XML element
                this.mod  = int(elem.getAttribute('mod'));
                this.sub  = int(elem.getAttribute('sub'));
                this.x    = int(elem.getAttribute('x'), x);
                this.z    = int(elem.getAttribute('y'), z); //XXX
                this.unk1 = int(elem.getAttribute('unk1'));
                this.unk2 = int(elem.getAttribute('unk2'));
            }
        }
    }
}

export default class Map {
    /** A map in the game.
     */
    constructor(app, elem) {
        this.app         = app;
        this.id          = null; //the map ID
        this.dirId       = null; //index into list of map dirs
        this.dirName     = null; //name of dir
        this.romListName = null; //name of romlist file
        this.name        = null; //internal name from MAPINFO.bin
        this.type        = null; //map type from MAPINFO.bin
        this.parentId    = null; //parent map ID
        this.playerObj   = null; //player object type (not used in this version)
        this.worldX      = null; //coords on global map grid
        this.worldZ      = null;
        this.layer       = null; //which global map grid
        this.sizeX       = null; //in blocks
        this.sizeZ       = null;
        this.originX     = null; //which block is the origin
        this.originZ     = null;
        this.links       = [null, null]; //linked map IDs
        this.blocks      = [];
        this.description = '';
        this.used        = null;

        if(elem) this._readXml(elem);
        if(this.id != null && this.app.game.iso) this._readGameFiles();
    }

    _readXml(elem) {
        //read info from XML element.
        this.id = int(elem.getAttribute('id'));
        this.dirId = int(elem.getAttribute('dirId'));
        this.type = int(elem.getAttribute('type'));
        //XXX
        this.playerObj = (int(elem.getAttribute('param3')) << 8) |
            int(elem.getAttribute('param4'));
        this.romListName = elem.getAttribute('romlist');
        this.dirName = elem.getAttribute('dir');
        this.name = elem.getAttribute('internalName');
        this.parentId = int(elem.getAttribute('parent'));
        this.worldX   = int(elem.getAttribute('worldX'));
        this.worldZ   = int(elem.getAttribute('worldY')); //XXX
        this.layer    = int(elem.getAttribute('layer'));
        this.links[0] = int(elem.getAttribute('link0'));
        this.links[1] = int(elem.getAttribute('link1'));
        this.sizeX    = int(elem.getAttribute('w'));
        this.sizeZ    = int(elem.getAttribute('h'));
        this.originX  = int(elem.getAttribute('originX'));
        this.originZ  = int(elem.getAttribute('originY')); //XXX
        this.used     = !int(elem.getAttribute('unused'));

        let eDesc = elem.getElementsByTagName('description');
        if(eDesc && eDesc[0]) this.description = eDesc[0].textContent;

        for(let block of elem.getElementsByTagName('block')) {
            this.blocks.push(new Block(this.app, this, elem));
        }
    }

    _readGameFiles() {
        //read info from the files in the ISO.
        this._readMapInfo();

        const mapsTab = this.app.game.iso.getFile('/MAPS.tab');
        const mapsBin = this.app.game.iso.getFile('/MAPS.bin');
        if(mapsTab && mapsBin) {
            const tab = new MapsTabEntry(mapsTab.getRawData(),
                this.id * MapsTabEntry._size);
            const bin = mapsBin.getRawData();
            //console.log("MapsTabEntry size=", MapsTabEntry._size);

            this.mapsBin0Offset      = tab.info;
            this.blocksOffset        = tab.blocks;
            this.rectOffsets         = tab.rects;
            this.romListHeaderOffset = tab.romList;

            this._readMapsBin0(bin, tab);
            //console.log(this);
            this._readBlocks(bin, tab);
            this._readRomListHeader(bin, tab);
            //XXX rects
        }
    }

    _readMapInfo() {
        const mapInfo = this.app.game.iso.getFile('/MAPINFO.bin');
        if(mapInfo) {
            const data = new MapInfoEntry(mapInfo.getRawData(),
                this.id * MapInfoEntry._size);
            this.name         = data.name;
            this.type         = data.type;
            this.mapInfoUnk1D = data.unk1D; //always 6
            this.playerObj    = data.objType;
        }
    }

    _readMapsBin0(bin, tab) {
        //console.log(`Read maps.bin item 0 from 0x${hex(tab.info)}`);
        const bin0        = new MapsBinEntry0(bin, tab.info);
        this.sizeX        = bin0.sizeX;
        this.sizeZ        = bin0.sizeZ;
        this.originX      = bin0.originX;
        this.originZ      = bin0.originZ;
        this.mapsBinUnk08 = bin0.unk08;
        this.mapsBinUnk0C = bin0.unk0C; //always 0?
        this.mapsBinUnk1C = bin0.unk1C;
        this.bin0flags    = bin0.flags;
    }

    _readBlocks(bin, tab) {
        this.blocks = []; //ignore those from XML
        let bx=0, bz=0;
        for(let iBlock=0; iBlock<this.sizeX * this.sizeZ; iBlock++) {
            try {
                const val = bin.getUint32((iBlock*4)+tab.blocks);
                let block = new Block(this.app, this, val, bx, bz);
                if(block.mod == 0xFF) block = null; //empty cell
                this.blocks.push(block);
                if(++bx >= this.sizeX) { bx = 0; bz++; }
            }
            catch(ex) {
                //some maps have bogus block count/size
                console.error("Error reading blocks for map", this, ex);
                break;
            }
        }
    }

    _readRomListHeader(bin, tab) {
        const magic = bin.getUint32(tab.romList);
        let compLen, rawLen;
        switch(magic) {
            case 0x5A4C4200: { //'ZLB\0'
                //XXX pretty sure there can never be a ZLB header here...
                const version = bin.getUint32(tab.romList+4);
                if(vesion != 1) {
                    console.warn(`Map 0x${hex(this.id)} has unexpected ZLB version 0x${hex(version)} for romlist`);
                }
                rawLen  = bin.getUint32(tab.romList + 0x08);
                compLen = bin.getUint32(tab.romList + 0x0C);
                break;
            }
            case 0xFACEFEED: {
                rawLen  = bin.getUint32(tab.romList + 0x04);
                compLen = bin.getUint32(tab.romList + 0x0C);
                const ext = bin.getUint32(tab.romList + 0x08);
                if(ext > 8) {
                    console.log(`Map 0x${hex(this.id)} has 0x${hex(ext)} words in FACEFEED @ 0x${hex(tab.romList)}`);
                }
                break;
            }
            default:
                console.warn(`Map 0x${hex(this.id)} has invalid magic 0x${hex(magic, 8)} for romlist`);
        }

        this.romListSize     = rawLen;
        this.romListCompSize = compLen;
    }
}
