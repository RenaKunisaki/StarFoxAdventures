import {get, validNumber} from '/js/Util.js';
import {TextureHeader} from './structs.js';
import SfaTexture from '../SfaTexture.js';

const ImageFormat = {
    I4:     0,
    I8:     1,
    IA4:    2,
    IA8:    3,
    RGB565: 4,
    RGB5A3: 5,
    RGBA32: 6,
    C4:     8,
    C8:     9,
    C14X2:  0xA,
    CMPR:   0xE,
};

export default class TextureLoader {
    /** Loads SFA texture from disc.
     */

    constructor(gx) {
        this.gx = gx;
    }

    async loadFromMap(path, id, group=1) {
        /** Load a texture from a map's TEXn.[bin,tab] files.
         *  path:  Path to directory containing the files.
         *  id:    Texture ID.
         *  group: Which texture set to load (0 or 1).
         */
        const tabName = `disc/${path}/TEX${group}.tab`;
        const binName = `disc/${path}/TEX${group}.bin`;
        const [nFrames, texOffs] = await this._getTabEntry(tabName, id);

        //now download bin file
        const binFile = new DataView((await get({
            path:         binName,
            mimeType:     'application/octet-stream',
            responseType: 'arraybuffer',
        })).response);

        //and read the header
        const frameOffsets = [];
        const offStrs = []; //debug
        for(let i=0; i<nFrames; i++) {
            frameOffsets.push(binFile.getUint32(texOffs + (i*4)));
            offStrs.push('0x'+(frameOffsets[i].toString(16).padStart(8,'0').toUpperCase()));
        }
        console.log("Frame offsets:", offStrs);

        //download and decompress the data
        let zlbOffs = texOffs;
        if(nFrames)  zlbOffs += ((nFrames+1) * 4);
        //const data = await this._decompressTexture(zlbOffs, binFile, binName);
        ////now data is the raw texture object, which has a header.
        //return this._buildTexture(data);

        if(group == 0) id |= 0x8000;
        const res  = await this._getPng(zlbOffs, binFile, binName, id);
        const info = JSON.parse((await get({path:`/texture/?get=info&id=${id}`})).responseText);
        res.id           = id;
        res.group        = group;
        res.format       = parseInt(info.format);
        res.offset       = parseInt(info.offset);
        res.nFrames      = parseInt(info.nFrames);
        res.packedLength = parseInt(info.packedLength);
        res.rawLength    = parseInt(info.rawLength);
        return res;
    }

    async _getTabEntry(tabName, id) {
        /** Get the entry from the TEXn.tab file for this texture.
         */
        //first, download tab file
        const tabFile = new DataView((await get({
            path:         tabName,
            mimeType:     'application/octet-stream',
            responseType: 'arraybuffer',
        })).response);

        //read offset
        const entry = tabFile.getUint32(id*4);

        //first two bits are unknown. next six are a count of mipmaps/anim frames,
        //then offset (divided by 2).
        let nFrames = (entry >> 24) & 0x3F;
        //if only one frame, there's no header listing the offset of each frame.
        //it's simplest to treat that as being 0 frames, since we only use
        //this number to figure how many offsets there are.
        if(nFrames == 1) nFrames = 0;
        return [
            nFrames, //nFrames
            (entry & 0xFFFFFF)*2, //texOffs
        ];
    }

    async _getPng(zlbOffs, binFile, binName, id) {
        //WebGL DXT1 seems to not work properly or not support this format
        //so we'll once again rely on the server to decode it for us.
        //const header  = new TextureHeader(data);
        //console.log("Texture header", header._toString());
        const texture = new SfaTexture(this.gx.context);
        return await texture.loadFromImage(`/texture/${binName}?offset=${zlbOffs}&id=${id}`);
    }

    async _decompressTexture(zlbOffs, binFile, binName) {
        /** Decompress texture object from binFile.
         */
        const magic   = binFile.getUint32(zlbOffs);
        const version = binFile.getUint32(zlbOffs+4);
        const outLen  = binFile.getUint32(zlbOffs+8);
        const compLen = binFile.getUint32(zlbOffs+12);
        if(magic != 0x5A4C4200) {
            throw new Error("Unexpected magic: 0x" + magic.toString(16));
        }
        if(version == 0x44495200) { // "DIR\0"
            //XXX this means data is NOT compressed, but I forget the specifics.
            throw new Error("DIR format not implemented");
        }
        else if(version != 1) {
            throw new Error("ZLB version is "+String(version)+", only 1 is supported");
        }

        //now "decompress" the image by asking the server to do it for us
        const offs = zlbOffs + 0x10;
        return new DataView((await get({
            path:         `/zlb/${binName}?offset=${offs}&length=${compLen}`,
            mimeType:     'application/octet-stream',
            responseType: 'arraybuffer',
        })).response);
    }

    _buildTexture(data) {
        /** Given the uncompressed texture data, build the Texture object.
         */
        const header  = new TextureHeader(data);
        console.log("Texture header", header._toString());
        const texture = new Texture(this.gx.context);

        return texture.loadDXT1(data, header.width, header.height);
    }
}
