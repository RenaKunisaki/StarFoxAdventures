import {get, int, hex, getDescriptionAndNotes} from './Util.js';
import {FaceFeedHeader, ZlbHeader} from './Structs.js';

let mapsBin = null; //MAPS.BIN content (downloaded as needed)

class MapBlock {
    /** A block of a map.
     */
    constructor(game, eBlock) {
        /** Construct a MapBlock from a 'block' element from maps.xml.
         */
        this.game = game;
        this.mod  = int(eBlock.getAttribute('mod'));
        this.sub  = int(eBlock.getAttribute('sub'));
        this.x    = int(eBlock.getAttribute('x'));
        this.y    = int(eBlock.getAttribute('y'));
        this.unk1 = int(eBlock.getAttribute('unk1'));
        this.unk2 = int(eBlock.getAttribute('unk2'));
    }
}

function readBlockRect(view, offs) {
    const val = view.getUint16(offs);
    return {
        x1: (val >> 0x0C & 0xF) - 7,
        y1: (val >> 0x08 & 0xF) - 7,
        x2: (val >> 0x04 & 0xF) - 7,
        y2: (val         & 0xF) - 7,
    };
}

export default class GameMap {
    /** A map in the game.
     */
    constructor(game, eMap) {
        /** Construct a GameMap from a 'map' element from maps.xml.
         */
        this.game         = game;
        this.id           = int(eMap.getAttribute('id'));
        this.dirId        = int(eMap.getAttribute('dirId'));
        this.type         = int(eMap.getAttribute('type'));
        this.param3       = int(eMap.getAttribute('param3'));
        this.param4       = int(eMap.getAttribute('param4'));
        this.romlistName  = eMap.getAttribute('romlist');
        this.dirName      = eMap.getAttribute('dir');
        this.internalName = eMap.getAttribute('internalName');
        this.parentId     = int(eMap.getAttribute('parent')); //NOTE: dir ID, not map ID
        this.worldX       = int(eMap.getAttribute('worldX'));
        this.worldY       = int(eMap.getAttribute('worldY'));
        this.layer        = int(eMap.getAttribute('layer'));
        this.link0        = int(eMap.getAttribute('link0')); //these are map IDs
        this.link1        = int(eMap.getAttribute('link1'));
        this.width        = int(eMap.getAttribute('w'));
        this.height       = int(eMap.getAttribute('h'));
        this.originX      = int(eMap.getAttribute('originX'));
        this.originY      = int(eMap.getAttribute('originY'));
        this.infoOffset   = int(eMap.getAttribute('infoOffset'));
        this.blockTable   = int(eMap.getAttribute('blockTable'));
        this.rects1       = int(eMap.getAttribute('rects1'));
        this.rects2       = int(eMap.getAttribute('rects2'));
        this.rects3       = int(eMap.getAttribute('rects3'));
        this.rects4       = int(eMap.getAttribute('rects4'));
        this.romlistOffs  = int(eMap.getAttribute('romlistOffs'));
        this.unk08        = int(eMap.getAttribute('unk08'));
        this.unk0C        = int(eMap.getAttribute('unk0C'));
        this.unk10        = int(eMap.getAttribute('unk10'));
        this.unk14        = int(eMap.getAttribute('unk14'));
        this.unk18        = int(eMap.getAttribute('unk18'));
        this.unk1C        = int(eMap.getAttribute('unk1C'));
        this.unk1E        = int(eMap.getAttribute('unk1E'));
        this.unused       = eMap.getAttribute('unused') == '1';
        getDescriptionAndNotes(this, eMap);

        this.blocks = [];
        for(let eBlock of eMap.getElementsByTagName('block')) {
            this.blocks.push(new MapBlock(game, eBlock));
        }

        //framework expects an ID
        this._id = this.id;
        if(this.id === null) this.id = this.romlistName;
    }

    async readMapsBin() {
        if(!mapsBin) {
            mapsBin = (await get({
                //XXX need different disc path per version
                path:         '/disc/MAPS.bin',
                mimeType:     'application/octet-stream',
                responseType: 'arraybuffer',
            })).response;
        }
        this.readBlockRects();

        //read romlist header, which tells how much memory is needed
        //to hold the romlist.
        const mapsBinView = new DataView(mapsBin);
        //only check half of magic because loljs
        const data = mapsBinView.getUint16(this.romlistOffs);
        switch(data) {
            case 0x5A4C: //ZLB
                this.romlistSize = (new ZlbHeader(mapsBin, this.romlistOffs)).rawLen;
                break;
            case 0xFACE: //FACEFEED
                this.romlistSize = (new FaceFeedHeader(mapsBin, this.romlistOffs)).rawLen;
                break;
            default:
                console.error(`Unrecognized data at romlist offset 0x${hex(this.romlistOffs,6)} in MAPS.bin: 0x${hex(data,4)}`);
        }
        console.log("romlist size is", this.romlistSize);
    }

    async readBlockRects() {
        if(this.rects) return this.rects;

        const mapsBinView = new DataView(mapsBin);
        const offsets = [this.rects1, this.rects2, this.rects3, this.rects4];

        const rects = [ [], [], [], [] ];
        for(let i=0; i<this.width*this.height; i++) {
            for(let r=0; r<4; r++) {
                let offs = offsets[r];
                rects[r].push(readBlockRect(mapsBinView, offs));
                rects[r].push(readBlockRect(mapsBinView, offs+2));
                rects[r].push(readBlockRect(mapsBinView, offs+4));
                rects[r].push(readBlockRect(mapsBinView, offs+6));
            }
        }

        this.rects = rects;
        return this.rects;
    }

    getBlockByPos(x, y) {
        /** Find a block at exactly the given coordinates.
         */
        for(let block of this.blocks) {
            if(block.x == x && block.y == y) return block;
        }
        return null;
    }
}
