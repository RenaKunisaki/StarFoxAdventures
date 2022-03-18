export class Type {
    /** A data type that a struct field can have. */
    static _size; //number of bytes

    get size() { return this.constructor._size; } //loljs

    fromBytes(data, offset=0, littleEndian=false) {
        /** Read this type from a data stream.
         *  @param {DataView} data View to read from.
         *  @param {int} offset Byte offset to read from.
         *  @param {bool} littleEndian whether to use little endian byte order.
         *  @returns The value read from the view.
         */
        throw new Error("Class has no fromBytes method");
    }

    toBytes(value, data=null, offset=0, littleEndian=false) {
        /** Pack this type into a data stream.
         *  @param value Value to write.
         *  @param {DataView} data View to write into.
         *  @param {int} offset Byte offset to write to.
         *  @param {bool} littleEndian whether to use little endian byte order.
         *  @returns The data view.
         *  @note If view is null, it creates a new buffer and view.
         */
        throw new Error("Class has no toBytes method");
    }

    toString(value) {
        /** Convert this type to a string, for debugging.
         *  @param value Value to convert.
         *  @returns {string} String representation.
         */
        return String(value);
    }

    _getBuf(data) {
        if(data) return data;
        return new ArrayBuffer(this.size);
    }
}

//primitive types

export class s8 extends Type {
    static _size = 1;
    fromBytes(data, offset=0, littleEndian=false) {
        return data.getInt8(offset);
    }
    toBytes(value, data=null, offset=0, littleEndian=false) {
        data = this._getBuf(data);
        data.setInt8(offset, value);
        return data;
    }
}
export class u8 extends Type {
    static _size = 1;
    fromBytes(data, offset=0, littleEndian=false) {
        return data.getUint8(offset);
    }
    toBytes(value, data=null, offset=0, littleEndian=false) {
        data = this._getBuf(data);
        data.setUint8(offset, value);
        return data;
    }
}
export class s16 extends Type {
    static _size = 2;
    fromBytes(data, offset=0, littleEndian=false) {
        return data.getInt16(offset, littleEndian);
    }
    toBytes(value, data=null, offset=0, littleEndian=false) {
        data = this._getBuf(data);
        data.setInt16(offset, value, littleEndian);
        return data;
    }
}
export class u16 extends Type {
    static _size = 2;
    fromBytes(data, offset=0, littleEndian=false) {
        return data.getUint16(offset, littleEndian);
    }
    toBytes(value, data=null, offset=0, littleEndian=false) {
        data = this._getBuf(data);
        data.setUint16(offset, value, littleEndian);
        return data;
    }
}
export class s32 extends Type {
    static _size = 4;
    fromBytes(data, offset=0, littleEndian=false) {
        return data.getInt32(offset, littleEndian);
    }
    toBytes(value, data=null, offset=0, littleEndian=false) {
        data = this._getBuf(data);
        data.setInt32(offset, value, littleEndian);
        return data;
    }
}
export class u32 extends Type {
    static _size = 4;
    fromBytes(data, offset=0, littleEndian=false) {
        return data.getUint32(offset, littleEndian);
    }
    toBytes(value, data=null, offset=0, littleEndian=false) {
        data = this._getBuf(data);
        data.setUint32(offset, value, littleEndian);
        return data;
    }
}
export class s64 extends Type {
    static _size = 8;
    fromBytes(data, offset=0, littleEndian=false) {
        return data.getBigInt64(offset, littleEndian);
    }
    toBytes(value, data=null, offset=0, littleEndian=false) {
        data = this._getBuf(data);
        data.setBigInt64(offset, value, littleEndian);
        return data;
    }
}
export class u64 extends Type {
    static _size = 8;
    fromBytes(data, offset=0, littleEndian=false) {
        return data.getBigUint64(offset, littleEndian);
    }
    toBytes(value, data=null, offset=0, littleEndian=false) {
        data = this._getBuf(data);
        data.setBigUint64(offset, value, littleEndian);
        return data;
    }
}
export class float extends Type {
    static _size = 4;
    fromBytes(data, offset=0, littleEndian=false) {
        return data.getFloat32(offset, littleEndian);
    }
    toBytes(value, data=null, offset=0, littleEndian=false) {
        data = this._getBuf(data);
        data.setFloat32(offset, value, littleEndian);
        return data;
    }
}
export class double extends Type {
    static _size = 8;
    fromBytes(data, offset=0, littleEndian=false) {
        return data.getFloat64(offset, littleEndian);
    }
    toBytes(value, data=null, offset=0, littleEndian=false) {
        data = this._getBuf(data);
        data.setFloat64(offset, value, littleEndian);
        return data;
    }
}
export class char extends Type {
    static _size = 1;
    fromBytes(data, offset=0, littleEndian=false) {
        let res = data.getInt8(offset);
        if(res == 0) return '';
        else return String.fromCharCode(res);
    }
    toBytes(value, data=null, offset=0, littleEndian=false) {
        if(value == '') value = 0;
        else if(value.length > 1) {
            throw new Error("More than one character specified for char");
        }
        else value = value.charCodeAt(0);
        data = this._getBuf(data);
        data.setInt8(offset, value);
        return data;
    }
}
export class bool extends Type {
    static _size = 1;
    fromBytes(data, offset=0, littleEndian=false) {
        let res = data.getInt8(offset);
        return (res != 0);
    }
    toBytes(value, data=null, offset=0, littleEndian=false) {
        data = this._getBuf(data);
        data.setInt8(offset, value ? 1 : 0);
        return data;
    }
}
export class widebool extends Type {
    /** bool padded to 4 bytes, seen often in 32-bit systems */
    static _size = 4;
    fromBytes(data, offset=0, littleEndian=false) {
        let res = data.getInt32(offset, littleEndian);
        return (res != 0);
    }
    toBytes(value, data=null, offset=0, littleEndian=false) {
        data = this._getBuf(data);
        data.setInt32(offset, value ? 1 : 0, littleEndian);
        return data;
    }
}

export const types = { //name => class, for XML
    s8:s8,       u8:u8,
    s16:s16,     u16:u16,
    s32:s32,     u32:u32,
    s64:s64,     u64:u64,
    float:float, double:double,
    char:char,   bool:bool, widebool:widebool,

    //common aliases
    f32:float, f64:double,
    byte:s8, sbyte:s8, ubyte:u8,
    short:s16, ushort:u16, word:s16, uword:u16,
    int:s32, uint:s32, dword:s32, udword:u32,
    int8_t:s8,   uint8_t:u8,
    int16_t:s16, uint16_t:u16,
    int32_t:s32, uint32_t:u32,
    int64_t:s64, uint64_t:u64,
    BOOL:widebool,
};
