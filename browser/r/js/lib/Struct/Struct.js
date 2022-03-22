import { Type } from "./Type.js";

export default class Struct extends Type {
    constructor(params) {
        //should be called by parseXmlStructs().
        super();
        this.name         = params.name;
        this.littleEndian = params.littleEndian;
        this.notes        = params.notes;
        this.description  = params.description;
        this.fields       = params.fields;
        this.fieldsByName = params.fieldsByName;
        this._size = 0;
        for(let field of this.fields) {
            this._size = Math.max(this._size, field.offset+field.size);
        }
    }

    get size() { return this._size; }

    fromBytes(view, offset=0) {
        /** Read this struct from a DataView.
         *  @param {DataView} view The view to read from.
         *  @param {int} offset The byte offset to read from.
         *  @returns {object} The values read from the view.
         */
        const result = {};
        for(let field of this.fields) {
            result[field.name] = field.fromBytes(view, offset+field.offset);
        }
        return result;
    }

    toBytes(value, view, offset=0) {
        /** Write this struct to a DataView.
         *  @param {object} value The values to write.
         *  @param {DataView} view The view to write to.
         *  @param {int} offset The byte offset to write to.
         *  @returns {DataView} The view that was written to.
         */
        let result = view;
        for(let field of this.fields) {
            result = field.toBytes(value[field.name], view, offset+field.offset);
        }
        return result;
    }

    valueToString(value) {
        /** Convert this struct to a string, for debugging.
         *  @param value Value to convert.
         *  @returns {string} String representation.
         */
        if(value == undefined) return `[struct ${this.name}]`;
        const result = [];
        const nameLen = Math.max(...this.fields.map(f => f.name.length));
        for(let field of this.fields) {
            const name = (field.name+':').padEnd(nameLen+2);
            const val  = field.type.toString(value[field.name]);
            result.push(name+val);
        }
        return result.join('\n');
    }
}

