import BinaryFile from '../../../../lib/BinaryFile.js';
import { VAT_FIELD_ORDER } from './GX.js';

export default class DlistParser {
    constructor() {
        this.regs = {
            bp: {},
            cp: {},
            xf: {},
        };
    }

    parse(list, buffers) {
        /** Parse a display list.
         *  @param {ArrayBuffer} list Display list to parse.
         *  @param {object} buffers Dict of vertex attribute buffers.
         *  @returns {object} Dict of generated buffers.
         */
        list = new BinaryFile(list);
        this.result = {};
        while(!list.isEof()) {
            const op = list.readU8();
            if(op >= 0x80 && op <= 0xC0) {
                this._parseDrawOp(op, list, buffers);
            }
            else switch(op) {
                case 0x00: break; //NOP
                case 0x08: { //LOAD_CP_REG
                    const reg = list.readU8();
                    const val = list.readU32();
                    this.regs.cp[reg] = val;
                    break;
                }
                case 0x10: { //LOAD_XF_REG
                    const nVals = (list.readU16() & 0xF) + 1;
                    const reg   = list.readU16();
                    for(let i=0; i<nVals; i++) {
                        this.regs.xf[reg+i] = list.readU32();
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
                    throw new Error(`Recursive display list call at offset 0x${hex(list.tell(),4)}`);
                case 0x48: //INVAL_CACHE
                    break; //nothing to do

                case 0x61: { //LOAD_BP_REG
                    let val = listData.readU32();
                    const reg = val >> 24;
                    val &= 0xFFFFFF;
                    this.regs.bp[reg] = val;
                    break;
                }
                default:
                    throw new Error(`Unknown display list opcode 0x${hex(op,2)} at offset 0x${hex(list.tell(),4)}`);
            }
        }
        return this.result;
    }

    _parseDrawOp(op, list, buffers) {
        const vat = this._getVat(op & 7);
        const nVtxs = list.readU16();
        switch(op & ~7) {
            case 0x80: case 0x88:
                this._drawQuads(vat, list, buffers, nVtxs);
                break;
            case 0x90: { //TRIANGLES
            }
            case 0x98: { //TRI_STRIP
            }
            case 0xA0: { //TRI_FAN
            }
            case 0xA8: { //LINES
            }
            case 0xB0: { //LINE_STRIP
            }
            case 0xB8: { //POINTS
            }
        }
    }

    _drawQuads(vat, list, buffers, nVtxs) {
        const vtxs = [];
        for(let i=0; i<nVtxs; i += 4) {

        }
        return vtxs;
    }

    _readVertex(vat, list, buffers) {
        const vtx = {
            _vat:  vat,  //debug
            _list: list, //debug
        };
        for(const field of VAT_FIELD_ORDER) {
            switch(fmt) {
                case 0: break; //no data
                case 1: //direct
                    vtx[field] = this._readImmediate(field, list, vat);
                    break;
                case 2:   //8-bit index
                case 3: { //16-bit index
                    const idx = (fmt == 2 ? dl.readU8() : dl.readU16());

                    //XXX this is the part to change
                    //store to buffers instead of directly to vtx

                    const src = this.data[field];
                    if(src == null) {
                        //throw new Error("Field is null: " + String(field));
                        vtx[field] = null;
                    }
                    else {
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
                        vtx[field] = this._nextValue(field, src, vcd);
                    }
                }
            }
        }
        return vtx;
    }

    _readImmediate(field, list, vat) {
        if(field.startsWith('COL')) return this._readColor(field, src, vcd);
        else if(field.startsWith('NRM')) return this._readNormal(field, src, vcd);
        else return this._readCoord(field, src, vcd);
    }

    _readCoord(field, src, vat) {
        //read one (X,Y) or (X,Y,Z) set, depending on count.
        //return as an array [X,Y] or [X,Y,Z]
        const shift  = vat[field+'SHFT'] || 0; //undefined => 0
        const fmt    = vat[field+'FMT']  || 0;
        const count  = vat[field+'CNT']  || 0;
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
            if((fmt == 0 || fmt == 1) && vat.BYTEDEQUANT) val /= (1 << shift);
            else if(fmt == 2 || fmt == 3) val /= (1 << shift);
            vals.push(val);
        }
        return vals;
    }

    _readNormal(field, src, vat) {
        //read one or three (X,Y,Z) sets, depending on count.
        //return as an array [X,Y,Z] or [X,Y,Z,X,Y,Z,X,Y,Z].
        //const shift  = vcd[field+'SHFT'] || 0; //not used for normals
        const format = vat[field+'FMT']  || 0; //undefined => 0
        const count  = vat[field+'CNT']  || 0;
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

    _readColor(field, src, vat) {
        //read one color value.
        //return as an array [R, G, B, A].
        //const shift  = vat[field+'SHFT'] || 0; //not used for colors
        const format = vat[field+'FMT']  || 0; //undefined => 0
        const count  = vat[field+'CNT']  || 0; //XXX how does this work for color?

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

    _getVat(vat) {
        //what the CNT values mean for different attrs
        const coordCounts    = [2, 3]; //(x,y), (x,y,z)
        const normalCounts   = [3, 9]; //(x,y,z) * (normal | normal,binormal,tangent)
        const texCoordCounts = [1, 2]; //(s), (s,t)
        const colorCounts    = [3, 4]; //(r,g,b), (r,g,b,a)
        /*
        //what the FMT values mean (makes more sense to use the raw values)
        const coordFmts  = ['u8',      's8', 'u16',     's16', 'float'];
        const normalFmts = [undefined, 's8', undefined, 's16', 'float'];
        const colorFmts  = [
            'rgb565', 'rgb888', 'rgbx8888', 'rgba4444', 'rgba6666', 'rgba8888'];
        //what the VCD values mean
        const vcdFmts = ['none', 'direct', 'u8', 'u16']; */

        const rVcdLo = this.regs.cp[0x50+vat];
        const rVcdHi = this.regs.cp[0x60+vat];
        const rVatA  = this.regs.cp[0x70+vat];
        const rVatB  = this.regs.cp[0x80+vat];
        const rVatC  = this.regs.cp[0x90+vat];
        const result = {
            _vatIdx:       vat, //debug
            PNMTXIDX:      (rVcdLo >>  0) & 0x01,
            POS:           (rVcdLo >>  9) & 0x03,
            NRM:           (rVcdLo >> 11) & 0x03,
            COL0:          (rVcdLo >> 13) & 0x03,
            COL1:          (rVcdLo >> 15) & 0x03,
            POSCNT:        (rVatA  >>  0) & 0x01,
            POSFMT:        (rVatA  >>  1) & 0x07,
            POSSHFT:       (rVatA  >>  4) & 0x1F,
            NRMCNT:        (rVatA  >>  9) & 0x01,
            NRMFMT:        (rVatA  >> 10) & 0x07,
            COL0CNT:       (rVatA  >> 13) & 0x01,
            COL0FMT:       (rVatA  >> 14) & 0x07,
            COL1CNT:       (rVatA  >> 17) & 0x01,
            COL1FMT:       (rVatA  >> 18) & 0x07,
            TEX0CNT:       (rVatA  >> 21) & 0x01,
            TEX0FMT:       (rVatA  >> 22) & 0x07,
            TEX0SHFT:      (rVatA  >> 25) & 0x1F,
            BYTEDEQUANT:   (rVatA  >> 30) & 0x01,
            NORMALINDEX3:  (rVatA  >> 31) & 0x01,
            TEX1CNT:       (rVatB  >>  0) & 0x01,
            TEX1FMT:       (rVatB  >>  1) & 0x07,
            TEX1SHFT:      (rVatB  >>  4) & 0x1F,
            TEX2CNT:       (rVatB  >>  9) & 0x01,
            TEX2FMT:       (rVatB  >> 10) & 0x07,
            TEX2SHFT:      (rVatB  >> 13) & 0x1F,
            TEX3CNT:       (rVatB  >> 18) & 0x01,
            TEX3FMT:       (rVatB  >> 19) & 0x07,
            TEX3SHFT:      (rVatB  >> 22) & 0x1F,
            TEX4CNT:       (rVatB  >> 27) & 0x01,
            TEX4FMT:       (rVatB  >> 28) & 0x07,
            VCACHE_ENHANCE:(rVatB  >> 31) & 0x01,
            TEX4SHFT:      (rVatC  >>  0) & 0x1F,
            TEX5CNT:       (rVatC  >>  5) & 0x01,
            TEX5FMT:       (rVatC  >>  6) & 0x07,
            TEX5SHFT:      (rVatC  >>  9) & 0x1F,
            TEX6CNT:       (rVatC  >> 14) & 0x01,
            TEX6FMT:       (rVatC  >> 15) & 0x07,
            TEX6SHFT:      (rVatC  >> 18) & 0x1F,
            TEX7CNT:       (rVatC  >> 23) & 0x01,
            TEX7FMT:       (rVatC  >> 24) & 0x07,
            TEX7SHFT:      (rVatC  >> 27) & 0x1F,
        };
        for(let i=0; i<8; i++) { //textures
            result[`T${i}MIDX`]  = (rVcdLo >> (i+1)) & 1;
            result[`TEX${i}`]    = (rVcdHi >> (i*2)) & 3;
            result[`TEX${i}CNT`] = texCoordCounts[result[`TEX${i}CNT`]];
        }
        //translate
        result.POSCNT  = coordCounts [result.POSCNT];
        result.NRMCNT  = normalCounts[result.NRMCNT];
        result.COL0CNT = colorCounts [result.COL0CNT];
        result.COL1CNT = colorCounts [result.COL1CNT];
        return result;
    }
}
