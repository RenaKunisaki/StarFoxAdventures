import {get, int, hex} from './Util.js';
//import {FaceFeedHeader, ZlbHeader} from './Structs.js';
import MapBlock from './MapBlock.js';

export default class ModFile {
    /** The contents of a `modxx.zlb.bin` file (map block data).
     */
    constructor(game) {
        /** Construct a ModFile, which must then be populated
         *  using the `load` method.
         */
        this.game = game;
        this.blocks = [];
    }

    async load(binPath, tabPath) {
        const tab = (await get({ //XXX per-version path
            path:         `/disc/${tabPath}`,
            mimeType:     'application/octet-stream',
            responseType: 'arraybuffer',
        })).response;
        const tabView = new DataView(tab);

        //load the blocks
        for(let i=0; i<tab.byteLength; i+=4) {
            let offs = tabView.getUint32(i);
            let next = tabView.getUint32(i+4);
            if(next == 0xFFFFFFFF) break;
            if((offs & 0xFF000000) == 0) {
                this.blocks.push(null);
                continue;
            }
            //the high byte has some flags but I don't think we need to care.
            offs &= 0x00FFFFFF;
            this.blocks.push(await this._loadBlock(binPath, offs,
                (next & 0x00FFFFFF) - offs));
        }

        return this;
    }

    async _loadBlock(binPath, offset, length) {
        //console.log(`Load block ${binPath}:0x${hex(offset,6)} len 0x${hex(length,6)}`);
        const data = (await get({
            path:         `/zlb/disc/${binPath}?offset=${offset+0x10}&length=${length}`,
            mimeType:     'application/octet-stream',
            responseType: 'arraybuffer',
        })).response;
        return new MapBlock(this, data);
    }
}
