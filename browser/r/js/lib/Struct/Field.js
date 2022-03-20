import { Type } from "./Type.js";

export default class Field extends Type {
    /** One field of a struct. */
    constructor(params) {
        /** Construct a Field.
         *  @param {int} offset The byte offset within the struct.
         *  @param {Type} type The data type.
         *  @param {string} name The field name.
         *  @param {bool} littleEndian whether to use little endian byte order.
         *  @param {int} count The array count, if applicable.
         *  @param {string} description A description of the field.
         *  @param {string[]} notes Some notes about the field.
         *  @note This should be called by the parser. You shouldn't
         *   need to call it yourself.
         */
        super();
        console.assert(params.type instanceof Type);
        console.assert(params.type.size);
        this.offset       = params.offset;
        this.type         = params.type;
        this.name         = params.name;
        this.count        = params.count;
        this.littleEndian = params.littleEndian;
        this._size        = this.type.size * this.count;
        this.description  = params.description;
        this.notes        = params.notes;
    }

    get size() { return this._size; }

    fromBytes(view, offset=0, littleEndian=undefined) {
        /** Read this field from a DataView.
         *  @param {DataView} view The view to read from.
         *  @param {int} offset The byte offset to read from.
         *  @param {bool} littleEndian whether to use little endian byte order.
         *  @returns The value read from the view.
         */
        if(littleEndian == undefined) littleEndian = this.littleEndian;
        if(this.count == 1) {
            return this.type.fromBytes(view, offset, littleEndian);
        }
        const result = [];
        const size   = this.type.size;
        for(let i=0; i<this.count; i++) {
            result.push(this.type.fromBytes(view, offset, littleEndian));
            offset += size;
        }
        return result;
    }

    toBytes(value, view, offset=0, littleEndian=undefined) {
        /** Write this field to a DataView.
        *  @param value Value to write.
         *  @param {DataView} data View to write into.
         *  @param {int} offset Byte offset to write to.
         *  @param {bool} littleEndian whether to use little endian byte order.
         *  @returns {DataView} The view that was written to.
         */
        if(littleEndian == undefined) littleEndian = this.littleEndian;
        if(this.count == 1) {
            return this.type.toBytes(value, view, offset, littleEndian);
        }
        else {
            let result = view;
            const size = this.type.size;
            console.assert(value.length == this.count);
            for(let i=0; i<this.count; i++) {
                result = this.type.toBytes(value[i], view, offset, littleEndian);
                offset += size;
            }
            return result;
        }
    }

    toString(value) {
        /** Convert this field to a string, for debugging.
         *  @param value Value to convert.
         *  @returns {string} String representation.
         */
        if(this.count == 1) return this.type.toString(value);
        console.assert(value.length == this.count);
        let result = [];
        let length = 1;
        for(let i=0; i<this.count; i++) {
            let s = this.type.toString(value[i]);
            result.push(s);
            length += s + 2;
            if(length >= 100) {
                result.push('...');
                break;
            }
        }
        return '[' + result.join(', ') + ']';
    }
}
