import {get, validNumber} from '/r/js/Util.js';

export default class Decompressor {
    /** Decompresses data from SFA files.
     */
    static decompress(data, offset=0) {
        /** Decompress data, if it's compressed.
         *  data: ArrayBuffer to read.
         *  offset: Offset of beginning of data.
         *  Returns uncompressed data.
         */
        let D = new Uint8Array(data, offset);
        //const head = (D[0] << 24) | (D[1] << 16) | (D[2] << 8) | D[3];
        //JS is a butt so we'll just check three bytes.
        const head = (D[0] << 16) | (D[1] << 8) | D[2];
        switch(head) {
            case 0xFACEFE: return Decompressor._decompFacefeed(data, offset);
            case 0x5A4C42: return Decompressor._decompZlb(data, offset);
            case 0xE0E0E0: // uncompressed
                // following is size, data offset
                // but unsure how to interpret offset
                throw new Error("E0E0E0E0 format not implemented");
            default:
                console.log("Header is 0x%s, assuming uncompressed",
                    head.toString(16));
                return D;
        }
    }

    static _decompFacefeed(data, offset) {
        /** Decompress data with "FACEFEED" header.
         */
        const D       = new Uint8Array(data, offset);
        const outLen  = (D[ 4] << 24) | (D[ 5] << 16) | (D[ 6] << 8) | D[ 7];
        const zlbOffs = (D[ 8] << 24) | (D[ 9] << 16) | (D[10] << 8) | D[11];
        const compLen = (D[12] << 24) | (D[13] << 16) | (D[14] << 8) | D[15];
        console.log("Decompress FACEFEED: outLen=0x%s zlbOffs=0x%s compLen=0x%s",
            outLen.toString(16), zlbOffs.toString(16), compLen.toString(16));
        //zlbOffs skips the ZLB header
        const input   = new Uint8Array(data, offset+0x24, compLen+0x10);

        const dump = [];
        for(let i=0; i<16; i++) dump.push(input[i].toString(16).padStart(2,'0'));
        console.log("Input is:", dump.join(' '));

        //oops none of the JS ZLIB libs work, lol
        //and there's no API in the browser to do it even though it has to
        //do it for compressed HTTP requests because lol
        //fuck it I'll do it server-side
        const output  = pako.inflateRaw(input, {raw:true});
        if(output.byteLength != outLen) {
            console.error("Decompressed length is 0x%s, should be 0x%s",
                output.byteLength.toString(16), outLen.toString(16));
        }
        return output;
    }

    static _decompZlb(data, offset) {
        /** Decompress data with "ZLB" header.
         */
        const D       = new Uint8Array(data, offset);
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
        const input   = new Uint8Array(data, offset+0x10, compLen);
        const output  = pako.inflateRaw(input);
        if(output.byteLength != outLen) {
            console.error("Decompressed length is 0x%s, should be 0x%s",
                output.byteLength.toString(16), outLen.toString(16));
        }
        return output;
    }
}
