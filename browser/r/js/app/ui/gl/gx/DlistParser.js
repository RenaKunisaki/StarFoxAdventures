import BinaryFile from '../../../../lib/BinaryFile.js';
import { VAT_FIELD_ORDER } from './GX.js';

export const DrawOpNames = ['Quads', 'Quads2', 'Tris', 'TriStrip',
    'TriFan', 'Lines', 'LineStrip', 'Points'];

export default class DlistParser {
    /** GameCube GX display list parser.
     *  This is a companion class of the main GX class.
     *  Give it a display list, and it parses the list and calls methods of
     *  the parent GX instance.
     */
    constructor(gx) {
        this.gx   = gx;
        this.data = {
            dlist: null, //display list being interpreted
            PNMTXIDX: null, //indexed attribute data
            T0MIDX:null, T1MIDX:null, T2MIDX:null, T3MIDX:null,
            T4MIDX:null, T5MIDX:null, T6MIDX:null, T7MIDX:null,
            POS:   null, NRM:   null, COL0:  null, COL1:  null,
            TEX0:  null, TEX1:  null, TEX2:  null, TEX3:  null,
            TEX4:  null, TEX5:  null, TEX6:  null, TEX7:  null,
        };
    }

    execute(list, data) {
        /** Execute display list.
         *  @param {ArrayBuffer} list List to execute.
         *  @param {object} data Dict of data sources for attribute array indices.
         */
        for(let [field, src] of Object.entries(data)) {
            if(src) {
                if(field == '_debug') this.data[field] = src;
                else this.data[field] = new BinaryFile(src);
            }
            else console.error("Field", field, "is", src);
        }
        this.curList = list;
        const dl = new BinaryFile(list);
        this.data.dlist = dl;
        while(!dl.isEof()) {
            const opcode = dl.readU8();
            if(opcode >= 0x80 && opcode < 0xC0) {
                this.op_draw(opcode);
            }
            else switch(opcode) {
                case 0x00: break; //NOP
                case 0x08: { //load CP reg
                    const reg = dl.readU8();
                    const val = dl.readU32();
                    console.log("SET CP %s to %s",
                        reg.toString(16), val.toString(16));
                    this.gx.cp.setReg(reg, val);
                    break;
                }
                case 0x10: { //load XF reg
                    const count  = (dl.readU16() & 0xF) + 1;
                    const offs   =  dl.readU16();
                    for(let i=0; i<count; i++) {
                        this.gx.xf.setReg(i+offs, dl.readU32());
                    }
                    break;
                }
                case 0x20:   //load IndxA (position mtx)
                case 0x28:   //load IndxB (normal mtx)
                case 0x30:   //load IndxC (post mtxs)
                case 0x38: { //load IndxD (lights)
                    const idx   = dl.readU16();
                    let   offs  = dl.readU16();
                    const size  = (offs >> 12) + 1;
                    offs = offs & 0x0FFF;
                    //XXX what to do with these?
                    console.log("SET INDX 0x%s offs 0x%s count %d",
                        ((opcode >> 3) - 4).toString(16), offs.toString(16), size);
                    break;
                }
                case 0x40: //call dlist (not allowed within a dlist)
                    throw new Error(`Recursive display list call at offset 0x${hex(listData.tell(),4)}`);
                case 0x48: //Invalidate vtx cache
                    //nothing to do
                    break;
                case 0x61: { //load BP reg
                    let val = listData.readU32();
                    const reg = val >> 24;
                    val &= 0xFFFFFF;
                    console.log(`SET BP 0x${hex(reg,2)} to 0x${hex(val,6)}`);
                    //XXX do something
                    break;
                }
                default:
                    throw new Error(`Unknown display list opcode 0x${hex(opcode,2)} at offset 0x${hex(listData.tell(),4)}`);
            }
        }
        //console.log("end of list");
    }

    op_draw(opcode) {
        /** Execute draw opcode.
         *  Called from execute().
         */
        const mode  = (opcode & 0x78) >> 3;
        const vat   = opcode & 7;
        let   count = this.data.dlist.readU16();
        const vtxs  = [];
        for(let i=0; i<count; i++) {
            const vtx = this._nextVertex(vat);
            vtx.index = i;
            vtxs.push(vtx);
        }
        this.gx._doDrawOp(mode, vtxs);
    }

    _nextColor(field, src, vcd) {
        //read one color value.
        //return as an array [R, G, B, A].
        //const shift  = vcd[field+'SHFT'] || 0; //not used for colors
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

    _nextNormal(field, src, vcd) {
        //read one or three (X,Y,Z) sets, depending on count.
        //return as an array [X,Y,Z] or [X,Y,Z,X,Y,Z,X,Y,Z].
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
                    console.log("VCD:", vcd);
            }
            vals.push(val);
        }
        return vals;
    }

    _nextCoord(field, src, vcd) {
        //read one (X,Y) or (X,Y,Z) set, depending on count.
        //return as an array [X,Y] or [X,Y,Z]
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

    _nextValue(field, src, vcd) {
        if(field.startsWith('COL')) return this._nextColor(field, src, vcd);
        else if(field.startsWith('NRM')) return this._nextNormal(field, src, vcd);
        else return this._nextCoord(field, src, vcd);
    }

    _nextVertex(vat) {
        /** Read vertex data from current offset using format
         *  specified by current VAT.
         */
        const vtx = {
            //these fields aren't really needed, just for debug.
            offset:    this.data.dlist.offset,
            vat:       vat,
            shader:    this.data.shader,
            shaderIdx: this.data.shaderIdx,
        };
        const vcd = this.gx.cp.vcd[vat];
        const dl  = this.data.dlist;
        for(const field of VAT_FIELD_ORDER) {
            const fmt = vcd[field];
            if(field.endsWith('IDX')) { //always 8-bit direct
                if(fmt) vtx[field] = dl.readU8();
                //else, no data
            }
            else switch(fmt) {
                case 0: break; //no data
                case 1: //direct
                    vtx[field] = this._nextValue(field, dl, vcd);
                    break;
                case 2:   //8-bit index
                case 3: { //16-bit index
                    const idx = (fmt == 2 ? dl.readU8() : dl.readU16());
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

                    //DEBUG - record more info for later examination
                    vtx[field+'_idx']   = idx;
                    vtx[field+'_fmt']   = fmt;
                    vtx[field+'_shift'] = vcd[field+'SHFT'];
                    vtx[field+'_cnt']   = vcd[field+'CNT'];
                    break;
                }
                case undefined:
                    throw new Error(`VCD format for ${field} is not defined in VAT ${vat}`);
                default: throw new Error(`BUG: impossible VCD format: ${fmt}`);
            }
        }
        vtx.dataLength = this.data.dlist.offset - vtx.offset;
        console.assert(vtx.POS);

        //debug: color by shader ID
        if(this.data._debug) {
            for(const [k,v] of Object.entries(this.data._debug)) vtx[k] = v;
            let c = vtx['COL0'];
            switch(this.data._debug.shaderIdx) {
                case 0:  c = [255,   0,   0, 255]; break;
                case 1:  c = [  0, 255,   0, 255]; break;
                case 2:  c = [  0,   0, 255, 255]; break;
                case 3:  c = [255,   0, 255, 255]; break;
                case 4:  c = [255, 255,   0, 255]; break;
                case 5:  c = [  0, 255, 255, 255]; break;
                default: c = [255,   0, 255, 255]; break;
            }
            //debugColor overrides normal color, but not texture
            //vtx['debugColor'] = c;
        }

        //keep stats
        if(this.gx.stats.PNMTXIDX[vtx.PNMTXIDX] == undefined) {
            this.gx.stats.PNMTXIDX[vtx.PNMTXIDX] = 0;
        }
        this.gx.stats.PNMTXIDX[vtx.PNMTXIDX]++;
        return vtx;
    }
}
