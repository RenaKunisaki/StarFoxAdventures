export class Type {
    /** A data type that a struct field can have. */
    static _size; //number of bytes

    get size() { return this.constructor._size; } //loljs

    constructor() { }

    fromBytes(view, offset=0, littleEndian=false) {
        /** Read this type from a data stream.
         *  @param {DataView} view View to read from.
         *  @param {int} offset Byte offset to read from.
         *  @param {bool} littleEndian whether to use little endian byte order.
         *  @returns The value read from the view.
         */
        throw new Error("Class has no fromBytes method");
    }

    arrayFromBytes(view, count, offset=0, littleEndian=false) {
        /** Read array of this type from a data stream.
         *  @param {DataView} view View to read from.
         *  @param {int} count Number of items to read.
         *  @param {int} offset Byte offset to read from.
         *  @param {bool} littleEndian whether to use little endian byte order.
         *  @returns The values read from the view.
         */
        const size   = this.size;
        console.assert(size > 0);
        const result = [];
        for(let i=0; i<count; i++) {
            result.push(this.fromBytes(view, offset, littleEndian));
            offset += size;
        }
        return result;
    }

    toBytes(value, view=null, offset=0, littleEndian=false) {
        /** Pack this type into a data stream.
         *  @param value Value to write.
         *  @param {DataView} view View to write into.
         *  @param {int} offset Byte offset to write to.
         *  @param {bool} littleEndian whether to use little endian byte order.
         *  @returns The data view.
         *  @note If view is null, it creates a new buffer and view.
         */
        throw new Error("Class has no toBytes method");
    }

    arrayToBytes(value, view=null, offset=0, littleEndian=false) {
        /** Pack array of this type into a data stream.
         *  @param {array} value Values to write.
         *  @param {DataView} view View to write into.
         *  @param {int} offset Byte offset to write to.
         *  @param {bool} littleEndian whether to use little endian byte order.
         *  @returns The data view.
         *  @note If view is null, it creates a new buffer and view.
         */
        const size = this.size;
        view = this._getBuf(view, value.length);
        for(let i=0; i<value.length; i++) {
            this.toBytes(value[i], view, offset, littleEndian);
            offset += size;
        }
        return view;
    }

    toString(value) {
        /** Convert this type to a string, for debugging.
         *  @param value Value to convert.
         *  @returns {string} String representation.
         */
        return String(value);
    }

    _getBuf(view, count=1) {
        if(view) return view;
        return new DataView(new ArrayBuffer(this.size * count));
    }
}

//primitive types

export class s8 extends Type {
    static _size = 1;
    fromBytes(view, offset=0, littleEndian=false) {
        return view.getInt8(offset);
    }
    toBytes(value, view=null, offset=0, littleEndian=false) {
        view = this._getBuf(view);
        view.setInt8(offset, value);
        return view;
    }
}
export class u8 extends Type {
    static _size = 1;
    fromBytes(view, offset=0, littleEndian=false) {
        return view.getUint8(offset);
    }
    toBytes(value, view=null, offset=0, littleEndian=false) {
        view = this._getBuf(view);
        view.setUint8(offset, value);
        return view;
    }
}
export class s16 extends Type {
    static _size = 2;
    fromBytes(view, offset=0, littleEndian=false) {
        return view.getInt16(offset, littleEndian);
    }
    toBytes(value, view=null, offset=0, littleEndian=false) {
        view = this._getBuf(view);
        view.setInt16(offset, value, littleEndian);
        return view;
    }
}
export class u16 extends Type {
    static _size = 2;
    fromBytes(view, offset=0, littleEndian=false) {
        return view.getUint16(offset, littleEndian);
    }
    toBytes(value, view=null, offset=0, littleEndian=false) {
        view = this._getBuf(view);
        view.setUint16(offset, value, littleEndian);
        return view;
    }
}
export class s32 extends Type {
    static _size = 4;
    fromBytes(view, offset=0, littleEndian=false) {
        return view.getInt32(offset, littleEndian);
    }
    toBytes(value, view=null, offset=0, littleEndian=false) {
        view = this._getBuf(view);
        view.setInt32(offset, value, littleEndian);
        return view;
    }
}
export class u32 extends Type {
    static _size = 4;
    fromBytes(view, offset=0, littleEndian=false) {
        return view.getUint32(offset, littleEndian);
    }
    toBytes(value, view=null, offset=0, littleEndian=false) {
        view = this._getBuf(view);
        view.setUint32(offset, value, littleEndian);
        return view;
    }
}
export class s64 extends Type {
    static _size = 8;
    fromBytes(view, offset=0, littleEndian=false) {
        return view.getBigInt64(offset, littleEndian);
    }
    toBytes(value, view=null, offset=0, littleEndian=false) {
        view = this._getBuf(view);
        view.setBigInt64(offset, value, littleEndian);
        return view;
    }
}
export class u64 extends Type {
    static _size = 8;
    fromBytes(view, offset=0, littleEndian=false) {
        return view.getBigUint64(offset, littleEndian);
    }
    toBytes(value, view=null, offset=0, littleEndian=false) {
        view = this._getBuf(view);
        view.setBigUint64(offset, value, littleEndian);
        return view;
    }
}
export class float extends Type {
    static _size = 4;
    fromBytes(view, offset=0, littleEndian=false) {
        return view.getFloat32(offset, littleEndian);
    }
    toBytes(value, view=null, offset=0, littleEndian=false) {
        view = this._getBuf(view);
        view.setFloat32(offset, value, littleEndian);
        return view;
    }
}
export class double extends Type {
    static _size = 8;
    fromBytes(view, offset=0, littleEndian=false) {
        return view.getFloat64(offset, littleEndian);
    }
    toBytes(value, view=null, offset=0, littleEndian=false) {
        view = this._getBuf(view);
        view.setFloat64(offset, value, littleEndian);
        return view;
    }
}
export class char extends Type {
    /** @note Using an array of char will give you, literally, an
     *  array of characters. For strings, use a string type instead.
     */
    static _size = 1;
    fromBytes(view, offset=0, littleEndian=false) {
        let res = view.getInt8(offset);
        if(res == 0) return '';
        else return String.fromCharCode(res);
    }
    toBytes(value, view=null, offset=0, littleEndian=false) {
        if(value == '') value = 0;
        else if(value.length > 1) {
            throw new Error("More than one character specified for char");
        }
        else value = value.charCodeAt(0);
        view = this._getBuf(view);
        view.setInt8(offset, value);
        return view;
    }
}
export class ascii extends char {
    /** String type that assumes one byte per character. */
    arrayFromBytes(view, count, offset=0, littleEndian=false) {
        return super.arrayFromBytes(view, count, offset, littleEndian).join('');
    }
}
export class stringWithNulls extends u8 {
    /** String type that uses UTF-8 encoding and
     *  allows embedded NUL characters.
     */
    arrayFromBytes(view, count, offset=0, littleEndian=false) {
        const result = [];
        for(let i=0; i<count; i++) {
            let nBytes = 0;
            let c = this.fromBytes(view, offset++);
            if     (c >= 0xF0) { nBytes=3; c &= 0x07 }
            else if(c >= 0xE0) { nBytes=2; c &= 0x0F }
            else if(c >= 0xC0) { nBytes=1; c &= 0x1F }

            while(nBytes > 0 && i < count) {
                i++;
                const c2 = this.fromBytes(view, offset++);
                if((c2 & 0xC0) != 0x80) { //not valid encoding
                    c = 0xFFFD; //replacement character
                    break;
                }
                else {
                    c = (c << 6) | (c2 & 0x3F);
                    nBytes--;
                }
            } //while
            if(nBytes > 0) c = 0xFFFD; //truncated character
            result.push(c);
        } //for
        return String.fromCodePoint(...result);
    }
    arrayToBytes(value, view=null, offset=0, littleEndian=false) {
        //expects value to be a string rather than an array
        view = this._getBuf(view, value.length);
        for(let i=0; i<value.length; i++) {
            let bytes;
            let code = value.codePointAt(i);
            if(code < 0x80) bytes = [code];
            else if(code < 0x800) bytes = [
                0xC0 | (code >> 6),
                0x80 | (code & 0x3F),
            ];
            else if(code < 0xD800 || code >= 0xE000) bytes = [
                0xE0 |  (code >> 12),
                0x80 | ((code >>  6) & 0x3F),
                0x80 |  (code & 0x3F),
            ];
            else { //UTF-16 surrogate pair
                let code2 = value.codePointAt(++i);
                if(code2 == undefined) code2 = 0xFFFD; //truncated character
                code = ((code & 0x3FF) << 10) | (code2 & 0x3FF);
                bytes = [
                    0xF0 |  (code >> 18),
                    0x80 | ((code >> 12) & 0x3F),
                    0x80 | ((code >>  6) & 0x3F),
                    0x80 |  (code        & 0x3F),
                ];
            }
            for(let b of bytes) {
                this.toBytes(b, view, offset++);
            }
        }
        return view;
    }
}
export class string extends stringWithNulls {
    /** String type that uses UTF-8 encoding and
     *  removes embedded NUL characters.
     */
    arrayFromBytes(view, count, offset=0, littleEndian=false) {
        const result = super.arrayFromBytes(view, count, offset, littleEndian);
        return result.replaceAll('\x00', '');
    }
}
export class bool extends Type {
    static _size = 1;
    fromBytes(view, offset=0, littleEndian=false) {
        let res = view.getUint8(offset);
        if(res > 1) console.warn(`bool value is 0x${res.toString(16)} at offset 0x${offset.toString(16)}`);
        return (res != 0);
    }
    toBytes(value, view=null, offset=0, littleEndian=false) {
        view = this._getBuf(view);
        view.setInt8(offset, value ? 1 : 0);
        return view;
    }
}
export class widebool extends Type {
    /** bool padded to 4 bytes, seen often in 32-bit systems */
    static _size = 4;
    fromBytes(view, offset=0, littleEndian=false) {
        let res = view.getInt32(offset, littleEndian);
        return (res != 0);
    }
    toBytes(value, view=null, offset=0, littleEndian=false) {
        view = this._getBuf(view);
        view.setInt32(offset, value ? 1 : 0, littleEndian);
        return view;
    }
}

export const types = { //name => class, for XML
    s8:s8,         u8:u8,
    s16:s16,       u16:u16,
    s32:s32,       u32:u32,
    s64:s64,       u64:u64,
    float:float,   double:double,
    char:char,     bool:bool, widebool:widebool,
    string:string, ascii:ascii, stringWithNulls:stringWithNulls,

    //common aliases
    f32:float,   f64:double,
    byte:s8,     sbyte:s8,   ubyte:u8,
    short:s16,   ushort:u16, word:s16,  uword:u16,
    int:s32,     uint:s32,   dword:s32, udword:u32,
    int8_t:s8,   uint8_t:u8,
    int16_t:s16, uint16_t:u16,
    int32_t:s32, uint32_t:u32,
    int64_t:s64, uint64_t:u64,
    BOOL:widebool,
};
