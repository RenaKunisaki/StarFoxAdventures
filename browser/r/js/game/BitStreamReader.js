import { assertType } from "../Util.js";

export default class BitStreamReader {
    /** Allows data to be read in arbitrary bit-lengths from a stream. */
    constructor(buffer) {
        /** Construct BitStreamReader.
         *  @param {ArrayBuffer} buffer Buffer to read from.
         */
        assertType(buffer, ArrayBuffer);
        this.buffer = new Uint8Array(buffer);
        this.offset = 0;
        this.length = this.buffer.byteLength * 8;
    }

    _getU24() {
        const byteOffs = this.offset >> 3;
        const shift    = this.offset  & 7;
        return (
            (this.buffer[byteOffs+2] << 16) |
            (this.buffer[byteOffs+1] <<  8) |
             this.buffer[byteOffs]
        ) >> shift;
    }

    read(size) {
        /** Read `size` bits from the stream and return them.
         */
        console.assert(size <= 24);
        const val = this._getU24() & ((1 << size)-1);
        this.offset += size;
        return val;
    }

    seek(offset, whence=0) {
        switch(whence) {
            case 0: //SEEK_SET
            case 'set':
                this.offset = offset;
                break;
            case 1: //SEEK_CUR
            case 'cur':
                this.offset += offset;
                break;
            case 2: //SEEK_END
            case 'end':
                this.offset = this.length - offset;
                break;
            default: throw new Error("Invalid `whence` parameter: "+String(whence));
        }
        if(this.offset < 0) this.offset = 0;
        if(this.offset > this.length) this.offset = this.length;
        return this.offset;
    }

    get isEof() {
        return this.offset >= this.length;
    }
}
