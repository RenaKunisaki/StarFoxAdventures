import BinaryFile from '../../../../lib/BinaryFile.js';
import { VAT_FIELD_ORDER } from './GX.js';
import RenderBatch from './RenderBatch.js';



export default class DlistParser {
    constructor(gx) {
        this.gx = gx;
        this.gl = gx.gl;
    }
    parse(list, buffers) {
        /** Parse a display list.
         *  @param {ArrayBuffer} list Display list to parse.
         *  @param {object} buffers Dict of vertex attribute buffers.
         *  @returns {RenderBatch} Generated render batch object.
         */
        list = new BinaryFile(list);
        this.buffers = {};
        for(const [name, buf] of Object.entries(buffers)) {
            console.assert(buf instanceof ArrayBuffer);
            this.buffers[name] = new BinaryFile(buf);
        }

        this.result = new RenderBatch(this.gx);
        while(!list.isEof()) {
            const op = list.readU8();
            if(op >= 0x80 && op <= 0xC0) {
                this._parseDrawOp(op, list);
            }
            else switch(op) {
                case 0x00: break; //NOP
                case 0x08: { //LOAD_CP_REG
                    this.gx.cp.setReg(list.readU8(), list.readU32());
                    break;
                }
                case 0x10: { //LOAD_XF_REG
                    const nVals = (list.readU16() & 0xF) + 1;
                    const reg   = list.readU16();
                    for(let i=0; i<nVals; i++) {
                        this.gx.xf.setReg(reg+i, list.readU32());
                    }
                    break;
                }
                case 0x20: //LOAD_INDX_A
                case 0x28: //LOAD_INDX_B
                case 0x30: //LOAD_INDX_C
                case 0x38: { //LOAD_INDX_D
                    const idx   = dl.readU16();
                    let   offs  = dl.readU16();
                    const size  = (offs >> 12) + 1;
                    offs = offs & 0x0FFF;
                    //XXX what to do with these?
                    break;
                }
                case 0x40: //CALL_DL
                    //we're already parsing a dlist so this is invalid
                    throw new Error(`Recursive display list call at offset 0x${hex(list.tell(),4)}`);
                case 0x48: //INVAL_CACHE
                    break; //nothing to do

                case 0x61: { //LOAD_BP_REG
                    let val = listData.readU32();
                    const reg = val >> 24;
                    val &= 0xFFFFFF;
                    //this.regs.bp[reg] = val; XXX
                    break;
                }
                default:
                    throw new Error(`Unknown display list opcode 0x${hex(op,2)} at offset 0x${hex(list.tell(),4)}`);
            }
        }
        this.result.finish();
        return this.result;
    }
    _parseDrawOp(op, list) {
        /** Parse a draw operation and add the vertices to this.result.
         *  @param {integer} op Opcode to parse.
         *  @param {BinaryFile} list List to read from.
         */
        const vat   = op & 7;
        const nVtxs = list.readU16();
        const gl    = this.gl;
        switch(op & ~7) {
            case 0x80: case 0x88: this._drawQuads(vat, list, nVtxs);      break;
            case 0x90: this._drawPolys(vat,list,nVtxs,gl.TRIANGLES);      break;
            case 0x98: this._drawPolys(vat,list,nVtxs,gl.TRIANGLE_STRIP); break;
            case 0xA0: this._drawPolys(vat,list,nVtxs,gl.TRIANGLE_FAN);   break;
            case 0xA8: this._drawPolys(vat,list,nVtxs,gl.LINES);          break;
            case 0xB0: this._drawPolys(vat,list,nVtxs,gl.LINE_STRIP);     break;
            case 0xB8: this._drawPolys(vat,list,nVtxs,gl.POINTS);         break;
        }
    }
    _drawQuads(vat, list, nVtxs) {
        /** Parse a QUADS draw operation.
         *  @param {integer} vat Which VAT to use.
         *  @param {BinaryFile} list List to read from.
         *  @param {integer} nVtxs Number of vertices to read.
         *  @note Reads vertices and converts to triangles.
         */
        const vtxs = [];
        for(let i=0; i<nVtxs; i += 4) {
            const v0 = this._readVertex(vat, list);
            const v1 = this._readVertex(vat, list);
            const v2 = this._readVertex(vat, list);
            const v3 = this._readVertex(vat, list);
            vtxs.push(v0, v1, v2, v1, v2, v3);
        }
        this._drawPoly(this.gl.TRIANGLES, ...vtxs);
    }
    _drawPolys(vat, list, nVtxs, mode) {
        /** Parse a draw operation other than QUADS.
         *  @param {integer} vat Which VAT to use.
         *  @param {BinaryFile} list List to read from.
         *  @param {integer} nVtxs Number of vertices to read.
         *  @param {integer} mode Which GL drawing mode to use.
         */
        const vtxs = [];
        for(let i=0; i<nVtxs; i++) {
            vtxs.push(this._readVertex(vat, list));
        }
        this.result.addVertices(mode, ...vtxs);
    }
    _readVertex(vat, list) {
        /** Read a vertex from the display list.
         *  @param {integer} vat Which VAT to use.
         *  @param {BinaryFile} list List to read from.
         *  @returns {object} The vertex attributes.
         */
        const vtx = {};
        const vcd = this.gx.cp.vcd[vat];
        for(const field of VAT_FIELD_ORDER) {
            const fmt = vcd[field];
            let val = null;
            switch(fmt) {
                case 0: break; //no data
                case 1: //direct
                    val = this._readAttr(field, list, vcd);
                    break;
                case 2:   //8-bit index
                case 3: { //16-bit index
                    const idx = (fmt == 2 ? list.readU8() : list.readU16());
                    const src = this.buffers[field];
                    if(src != null) {
                        let stride = this.gx.cp.arrayStride[field];
                        if(stride == undefined) {
                            console.error("No array stride for field", field);
                            stride = 1;
                        }
                        else if(stride == 0) {
                            console.warn("Array stride is zero for field", field);
                        }
                        else if(stride < 0) {
                            console.error("Negative array stride for field", field);
                            stride = 1;
                        }
                        src.seek(idx * stride);
                        val = this._readAttr(field, src, vcd);
                    }
                }
            }
            vtx[field] = val;
        }
        return vtx;
    }
    _readAttr(field, src, vcd) {
        /** Read an attribute value from the given source.
         *  @param {string} field Attribute name to read.
         *  @param {BinaryFile} src Source to read from.
         *  @param {object} vcd The VCD to use.
         *  @returns {Array} Attribute value.
         */
        if(field.startsWith('COL')) return this._readColor(field, src, vcd);
        else if(field.startsWith('NRM')) return this._readNormal(field, src, vcd);
        else return this._readCoord(field, src, vcd);
    }
    _readCoord(field, src, vcd) {
        /** Read a coordinate value from the given source.
         *  @param {string} field Attribute name to read.
         *  @param {BinaryFile} src Source to read from.
         *  @param {object} vcd The VCD to use.
         *  @returns {Array} Either [X,Y] or [X,Y,Z] depending on VCD.
         */
        const shift  = vcd[field+'SHFT'] || 0; //undefined => 0
        const fmt    = vcd[field+'FMT']  || 0;
        const count  = vcd[field+'CNT']  || 0;
        const vals   = [];
        const cntMin = (field.startsWith('TEX') ? 1 : 2);
        for(let i=0; i<count + cntMin; i++) {
            let val = null;
            switch(fmt) {
                case 0: val = src.readU8();    break;
                case 1: val = src.readS8();    break;
                case 2: val = src.readU16();   break;
                case 3: val = src.readS16();   break;
                case 4: val = src.readFloat(); break;
                default: console.error(
                    "Invalid format for attribute %s (fmt=%s count=%s shift=%s)",
                    field, fmt, count, shift);
            }
            if((fmt == 0 || fmt == 1) && vcd.BYTEDEQUANT) val /= (1 << shift);
            else if(fmt == 2 || fmt == 3) val /= (1 << shift);
            vals.push(val);
        }
        return vals;
    }
    _readNormal(field, src, vcd) {
        /** Read a normal-vector value from the given source.
         *  @param {string} field Attribute name to read.
         *  @param {BinaryFile} src Source to read from.
         *  @param {object} vcd The VCD to use.
         *  @returns {Array} Either [X,Y,Z] or [X,Y,Z,X,Y,Z,X,Y,Z]
         *   depending on VCD.
         */
        //const shift  = vcd[field+'SHFT'] || 0; //not used for normals
        const format = vcd[field+'FMT']  || 0; //undefined => 0
        const count  = vcd[field+'CNT']  || 0;
        const vals   = [];
        for(let i=0; i<(count ? 9 : 3); i++) {
            let val = null;
            switch(format) {
                //u8, u16 are not valid for normals
                case 1: val = src.readS8()  / (1<< 6); break;
                case 3: val = src.readS16() / (1<<14); break;
                case 4: val = src.readFloat();     break;
                default:
                    console.error("Invalid format for attribute %s (fmt=%s count=%s)",
                        field, format, count);
            }
            vals.push(val);
        }
        return vals;
    }
    _readColor(field, src, vcd) {
        /** Read a color value from the given source.
         *  @param {string} field Attribute name to read.
         *  @param {BinaryFile} src Source to read from.
         *  @param {object} vcd The VCD to use.
         *  @returns {Array} [R, G, B, A].
         */
        //const shift  = vat[field+'SHFT'] || 0; //not used for colors
        const format = vcd[field+'FMT']  || 0; //undefined => 0
        const count  = vcd[field+'CNT']  || 0; //XXX how does this work for color?

        let r=0, g=0, b=0, a=255;
        switch(format) {
            case 0: { //RGB565
                let v = src.readU16();
                b = ( v        & 0x1F) * (255/31);
                g = ((v >>  5) & 0x3F) * (255/63);
                r = ((v >> 11) & 0x1F) * (255/31);
                break;
            }
            case 1: { //RGB888
                r = src.readU8();
                g = src.readU8();
                b = src.readU8();
                break;
            }
            case 2: { //RGBX8888
                r = src.readU8();
                g = src.readU8();
                b = src.readU8();
                src.readU8(); //discard
                break;
            }
            case 3: { //RGBA4444
                let v = src.readU16();
                a = ( v        & 0xF) * (255/15);
                b = ((v >>  4) & 0xF) * (255/15);
                g = ((v >>  8) & 0xF) * (255/15);
                r = ((v >> 12) & 0xF) * (255/15);
                break;
            }
            case 4: { //RGBA6666
                let v = (src.readU8() << 16) | src.readU16();
                a = ( v        & 0x3F) * (255/63);
                b = ((v >>  6) & 0x3F) * (255/63);
                g = ((v >> 12) & 0x3F) * (255/63);
                r = ((v >> 18) & 0x3F) * (255/63);
                break;
            }
            case 5: { //RGBA8888
                r = src.readU8();
                g = src.readU8();
                b = src.readU8();
                a = src.readU8();
                break;
            }
            default:
                console.error("Invalid format for attribute %s (fmt=%s count=%s)",
                    field, format, count);
                return null;
        }
        return [r, g, b, a];
    }
}
