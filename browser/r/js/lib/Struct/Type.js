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

    toString(value) {
        /** Convert this type to a string, for debugging.
         *  @param value Value to convert.
         *  @returns {string} String representation.
         */
        return String(value);
    }

    _getBuf(view) {
        if(view) return view;
        return new DataView(new ArrayBuffer(this.size));
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
export class bool extends Type {
    static _size = 1;
    fromBytes(view, offset=0, littleEndian=false) {
        let res = view.getInt8(offset);
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
