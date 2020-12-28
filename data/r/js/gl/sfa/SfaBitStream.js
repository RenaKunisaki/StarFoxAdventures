import Seekable from '/r/js/Seekable.js';

export default class SfaBitStream extends Seekable {
    /** Variant of BitStream that reads in the order SFA does.
     */
    constructor(data) {
        /** Construct SfaBitStream.
         *  data: ArrayBuffer to read from.
         */
        super();
        this.buffer = new Uint8Array(data);
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
        const val = this._getU24() & ((1 << size)-1);
        this.offset += size;
        return val;
    }
}
