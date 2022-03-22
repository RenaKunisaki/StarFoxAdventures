import { Type } from "./Type.js";

export class EnumItem {
    constructor(params) {
        this.name         = params.name;
        this.value        = params.value;
        this.notes        = params.notes;
        this.description  = params.description;
    }
}

export class Enum extends Type {
    constructor(params) {
        //should be called by parseXmlStructs().
        super();
        this.name         = params.name;
        this.type         = params.type;
        this.fields       = params.fields; //val => name
        this.fieldsByName = params.fieldsByName; //name => val
        this.notes        = params.notes;
        this.description  = params.description;
    }

    get size() { return this.type.size; }

    fromBytes(view, offset=0, littleEndian=false) {
        /** Read this enum from a DataView.
         *  @param {DataView} view The view to read from.
         *  @param {int} offset The byte offset to read from.
         *  @returns {object} The values read from the view.
         */
        const val = this.type.fromBytes(view, offset);
        if(this.fields[val] != undefined) return this.fields[val].value;
        return val;
    }

    toBytes(value, view=null, offset=0, littleEndian=false) {
        /** Write this enum to a DataView.
         *  @param {object} value The values to write.
         *  @param {DataView} view The view to write to.
         *  @param {int} offset The byte offset to write to.
         *  @returns {DataView} The view that was written to.
         */
        if(!this.isValidValue(value)) {
            throw new RangeError(`Invalid value '${value}' for enum '${this.name}'`);
        }
        return this.type.toBytes(this.fieldsByName[value].value, view, offset);
    }

    isValidValue(value) {
        return this.fieldsByName[value] != undefined;
    }

    valueToString(value) {
        /** Convert this enum to a string, for debugging.
         *  @param value Value to convert.
         *  @returns {string} String representation.
         */
        if(this.isValidValue(value)) return this.fields[value].name;
        else return String(value);
    }
}
