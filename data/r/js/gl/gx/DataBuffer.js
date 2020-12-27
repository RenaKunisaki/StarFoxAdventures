export default class DataBuffer {
    /** Generic data buffer.
     *  Contains binary data which is read sequentially or randomly.
     */
    constructor(src, order='>') {
        /** Construct DataBuffer.
         *  src: ArrayBuffer to wrap.
         *  order: '>' for big endian or '<' for little endian.
         */
        try {
            this.data   = new DataView(src);
        }
        catch(ex) {
            debugger;
        }
        this.offset = 0;  //current offset to read from
        this.length = this.data.byteLength;
        this.order  = order;
    }

    seek(offset, whence=0) {
        /** Set read position.
         *  offset: What to set position to or adjust position by.
         *  whence: How to adjust position:
         *    0 or 'set': position = offset
         *    1 or 'cur': position += offset
         *    2 or 'end': position = EOF - offset
         *  Returns new offset.
         */
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
        /** Check if read position is at end of stream.
         */
        return this.offset >= this.length;
    }

    nextS8() {
        /** Read next byte from data, increment offset,
         *  and return the byte.
         *  Return null if at end of data.
         */
        if(this.offset >= this.length) return null;
        return this.data.getInt8(this.offset++);
    }
    nextU8() {
        if(this.offset >= this.length) return null;
        return this.data.getUint8(this.offset++);
    }
    nextS16() {
        if(this.offset+1 >= this.length) return null;
        this.offset += 2;
        return this.data.getInt16(this.offset-2, this.order=='<');
    }
    nextU16() {
        if(this.offset+1 >= this.length) return null;
        this.offset += 2;
        return this.data.getUint16(this.offset-2, this.order=='<');
    }
    nextS32() {
        if(this.offset+3 >= this.length) return null;
        this.offset += 4;
        return this.data.getInt32(this.offset-4, this.order=='<');
    }
    nextU32() {
        if(this.offset+3 >= this.length) return null;
        this.offset += 4;
        return this.data.getUint32(this.offset-4, this.order=='<');
    }
    nextS64() {
        if(this.offset+7 >= this.length) return null;
        this.offset += 8;
        return this.data.getBigInt64(this.offset-8, this.order=='<');
    }
    nextU64() {
        if(this.offset+7 >= this.length) return null;
        this.offset += 8;
        return this.data.getBigUint64(this.offset-8, this.order=='<');
    }
    nextFloat() {
        if(this.offset+3 >= this.length) return null;
        this.offset += 4;
        return this.data.getFloat32(this.offset-4, this.order=='<');
    }
    nextDouble() {
        if(this.offset+7 >= this.length) return null;
        this.offset += 8;
        return this.data.getFloat64(this.offset-8, this.order=='<');
    }
}
