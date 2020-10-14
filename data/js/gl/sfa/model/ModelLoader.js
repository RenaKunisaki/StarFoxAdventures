import {get, validNumber} from '/js/Util.js';
import Decompressor from '/js/gl/sfa/Decompressor.js';
import Model from './Model.js';
import Texture from '/js/gl/Texture.js';

export default class ModelLoader {
    /** Loads SFA model from disc.
     */
    constructor(gx) {
        this.gx = gx;
    }

    async loadFromModelsBin(path, offset) {
        /** Load a model from MODELS.BIN file.
         *  path: Path to file.
         *  offset: Location of model within file.
         */
        const compData = (await get({
            path:         path,
            mimeType:     'application/octet-stream',
            responseType: 'arraybuffer',
        })).response;

        //we'll just use this hack for now until maybe someday we have
        //an actual working ZLB decompressor in JS.
        let data;
        let D = new Uint8Array(compData, offset);
        //const head = (D[0] << 24) | (D[1] << 16) | (D[2] << 8) | D[3];
        //JS is a butt so we'll just check three bytes.
        const head = (D[0] << 16) | (D[1] << 8) | D[2];
        switch(head) {
            case 0xFACEFE: data = await this._loadFaceFeed(path, D, offset); break;
            case 0x5A4C42: data = await this._loadZlb(path, d, offset); break;
            case 0xE0E0E0: // uncompressed
                // following is size, data offset
                // but unsure how to interpret offset
                throw new Error("E0E0E0E0 format not implemented");
            default:
                console.log("Header is 0x%s, assuming uncompressed",
                    head.toString(16));
                data = D;
        }
        const model = new Model(this.gx);
        model._parseData(data);
        await model._downloadTextures();
        return model;
    }

    async _loadFaceFeed(path, D, offset) {
        /** Load model with FACEFEED header.
         */
        const outLen  = (D[ 4] << 24) | (D[ 5] << 16) | (D[ 6] << 8) | D[ 7];
        const zlbOffs = (D[ 8] << 24) | (D[ 9] << 16) | (D[10] << 8) | D[11];
        const compLen = (D[12] << 24) | (D[13] << 16) | (D[14] << 8) | D[15];
        console.log("Decompress FACEFEED: outLen=0x%s zlbOffs=0x%s compLen=0x%s",
            outLen.toString(16), zlbOffs.toString(16), compLen.toString(16));

        //zlbOffs skips the ZLB header
        const offs = offset  + (zlbOffs*4)+4;
        const len  = compLen + 0x10;
        console.log("Decompress offs=0x%s", offs.toString(16));

        //just get the raw data from the server.
        path = `/zlb/${path}?offset=${offs}&length=${len}&`;
        return (await get({
            path:         path,
            mimeType:     'application/octet-stream',
            responseType: 'arraybuffer',
        })).response;
    }

    async _loadZlb(path, D, offset) {
        /** Load model with ZLB header.
         */
        const version = (D[ 4] << 24) | (D[ 5] << 16) | (D[ 6] << 8) | D[ 7];
        const outLen  = (D[ 8] << 24) | (D[ 9] << 16) | (D[10] << 8) | D[11];
        const compLen = (D[12] << 24) | (D[13] << 16) | (D[14] << 8) | D[15];
        console.log("Decompress ZLB: outLen=0x%s version=0x%s compLen=0x%s",
            outLen.toString(16), version.toString(16), compLen.toString(16));
        if(version == 0x44495200) { // "DIR\0"
            //XXX this means data is NOT compressed, but I forget the specifics.
            throw new Error("DIR format not implemented");
        }
        else if(version != 1) {
            throw new Error("ZLB version is "+String(version)+", only 1 is supported");
        }

        //just get the raw data from the server.
        const offs = offset + 0x10;
        path = `/zlb/${path}?offset=${offs}&length=${compLen}`;
        return (await get({
            path:         path,
            mimeType:     'application/octet-stream',
            responseType: 'arraybuffer',
        })).response;
    }
}
//0901C0
