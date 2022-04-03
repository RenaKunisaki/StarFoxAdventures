import Field from "./Field.js";
import { u8 } from "./Type.js";
import { hex } from "../../Util.js";

export default class Padding extends Field {
    /** A field that isn't used. */
    constructor(params) {
        /** Construct a Field.
         *  @param {int} offset The byte offset within the struct.
         *  @param {int} count The number of padding bytes.
         *  @param {int} value The expected value.
         *  @param {string} description A description of the field.
         *  @param {string[]} notes Some notes about the field.
         *  @note This should be called by the parser. You shouldn't
         *   need to call it yourself.
         */
        super({
            offset:      params.offset,
            count:       params.count,
            description: params.description,
            notes:       params.notes,
            type:        new u8(),
        });
        this.value = params.value;
    }

    get typeName() { return `Padding(${this.count})` }

    fromBytes(view, offset=0, littleEndian=undefined) {
        const res = super.fromBytes(view, offset, littleEndian);
        if(this.value == undefined) return res;
        for(let i=0; i<this.count; i++) {
            if(res[i] != this.value) {
                console.warn(`Padding field has value 0x${hex(res[i],2)} at offset 0x${hex(offset+i)} (expected 0x${hex(this.value, 2)})`);
                break;
            }
        }
        return res;
    }

    toBytes(value, view, offset=0, littleEndian=undefined) {
        const v = (this.value == undefined) ? 0 : this.value;
        if(this.count == 1) {
            return this.type.toBytes(v, view, offset, littleEndian);
        }

        const r = new ArrayBuffer(this.count);
        for(let i=0; i<this.count; i++) r[i] = v;
        const nv = new DataView(r);
        return this.type.arrayToBytes(nv, view, offset, littleEndian);
    }

    valueToString(value) {
        return "[padding]";
    }
}
