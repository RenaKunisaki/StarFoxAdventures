import Seekable from '/js/Seekable.js';

export default class BitStream extends Seekable {
    /** Wraps an ArrayBuffer and provides sequential access to it in
     *  variable-sized chunks of bits.
     */
    constructor(data, order='<') {
        /** Construct BitStream.
         *  data: ArrayBuffer to read from.
         *  order: Which order to read bits from each byte.
         *    '<': LSB (0x01) first;  '>': MSB (0x80) first
         */
        //this.data   = data;
        super();
        this.buffer = new Uint8Array(data);
        this.order  = order;
        this.offset = 0;
        this.length = this.buffer.byteLength * 8;
    }

    read(size) {
        /** Read `size` bits from the stream and return them.
         */
        const newOffs = this.offset + size;
        if(newOffs >= this.length) return null;
        let res = 0;
        for(let i=0; i<size; i++) {
            let byte = this.buffer[(this.offset+i) >> 3];
            let bitIdx = (this.offset+i) & 7;
            if(this.order == '>') byte >>= (7-bitIdx);
            else byte >>= bitIdx;
            res = (res << 1) | (byte & 1);
        }
        this.offset = newOffs;
        return res;
    }
}
