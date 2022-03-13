export default class BytesIO {
    ///mimicks Python's BytesIO enough for this code.
    constructor(bufSize=256) {
        this._data = new ArrayBuffer(bufSize);
        this._offset = 0;
        this._length = 0;
    }

    seek(offset) {
        this._offset = offset;
    }

    write(data) {
        if(data.length + this._offset >= this._data.byteLength) {
            if(ArrayBuffer.transfer) {
                this._data = ArrayBuffer.transfer(
                    this._data, data.length + this._offset + 64);
            }
            else {
                let newBuf = new ArrayBuffer(data.length + this._offset + 64);
                for(let i=0; i<this._length; i++) {
                    newBuf[i] = this._data[i];
                }
                this._data = newBuf;
            }
        }
        for(let i=0; i<data.length; i++) {
            //XXX faster way?
            this._data[this._offset++] = data[i];
        }
        this._length = Math.max(this._length, this._offset);
    }

    read() {
        return this._data.slice(this._offset, this._length - this._offset);
    }

    read_u8(offset=null) {
        if(offset != null) this.seek(offset);
        return this._data[this._offset];
    }
    read_u16(offset=null) {
        if(offset != null) this.seek(offset);
        return (this._data[this._offset] << 8) | this._data[this._offset+1];
    }
    read_u32(offset=null) {
        if(offset != null) this.seek(offset);
        return (
            (this._data[this._offset  ] << 24) | (this._data[this._offset+1] << 16)
            (this._data[this._offset+2] <<  8) |  this._data[this._offset+3]);
    }
}
