import { hsv2rgb } from '../../Util.js';

//struct types
let HitsBinEntry;

export default class BlockHit {
    /** An entry from HITS.bin. */
    constructor(block, entry, idx, offset) {
        /** Construct from HitsBinEntry.
         *  @param {Block} block Map block this entry belongs to.
         *  @param {HitsBinEntry} entry Entry to construct from.
         *  @param {integer} idx Which index this entry is in the block's array.
         *  @param {integer} offset Where this entry is in HITS.bin.
         */
        this.game   = block.game;
        this.app    = this.game.app;
        this.block  = block;
        this.index  = idx;
        this.offset = offset;

        if(!HitsBinEntry) {
            HitsBinEntry = this.app.types.getType('sfa.maps.HitsBinEntry');
        }

        this.coords = [
            [entry.x1, entry.y1, entry.z1],
            [entry.x2, entry.y2, entry.z2],
        ];
        this.type = entry.type & 0x3F;
        this.unkTypeBits = entry.type >> 6;
        this.flags = {
            oneHeight:    (entry.flags & 0x80) != 0,
            unk40:        (entry.flags & 0x40) != 0,
            unk20:        (entry.flags & 0x20) != 0,
            unk10:        (entry.flags & 0x10) != 0,
            unk08:        (entry.flags & 0x08) != 0,
            unk04:        (entry.flags & 0x04) != 0,
            unk02:        (entry.flags & 0x02) != 0,
            ignorePlayer: (entry.flags & 0x01) != 0,
        };
        this.unk10 = entry._10;
        this.unk12 = entry._12;
        let h1, h2;
        if(this.flags.oneHeight) {
            h1 = (entry.height[0] << 8) | entry.height[1];
            h2 = h1;
        }
        else {
            h1 = entry.height[0];
            h2 = entry.height[1];
        }
        this.height = [h1, h2];
    }

    toVtxs(gx, id=null) {
        const vtxs = [gx.gl.TRIANGLES];
        const idxs = [0,1,2,2,3,0];
        const [x1,y1,z1] = this.coords[0];
        const [x2,y2,z2] = this.coords[1];
        let   [h1,h2]    = this.height;

        //minimum height so we can see it
        h1 = Math.max(h1, 8);
        h2 = Math.max(h2, 8);

        const vPos = [
            [x1, y1,    z1],
            [x1, y1+h1, z1],
            [x2, y2+h2, z2],
            [x2, y2,    z2],
        ];
        const [r,g,b] = hsv2rgb(((this.type / 0x1F)*360) % 360,
            (this.type & 0x40) ? 0.25 : 1, 1);
        const color = [r*255, g*255, b*255, 0xA0 ];
        for(let idx of idxs) {
            vtxs.push({
                POS: vPos[idx],
                COL0: color, COL1: color,
                id: id,
            });
        }
        return vtxs;
    }
}
