import DataBuffer from './DataBuffer.js';
import {hexdump} from '/r/js/Util.js';

const DrawOpNames = ['Quads', 'Quads2', 'Tris', 'TriStrip',
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
                else this.data[field] = new DataBuffer(src);
            }
            else console.error("Field", field, "is", src);
        }
        this.curList = list;
        this.data.dlist = new DataBuffer(list);
        //we also store the parsed opcodes for later examination.
        list.parsedOps = [];
        const dl = this.data.dlist;
        while(!dl.isEof) {
            const opcode = dl.nextU8();
            //console.log("DL %s op %s", (dl.offset-1).toString(16), opcode.toString(16));
            if(opcode >= 0x80 && opcode < 0xC0) {
                this.op_draw(opcode);
            }
            else switch(opcode) {
                case 0x00: { //NOP
                    let lastOp = null;
                    if(list.parsedOps.length) {
                        //if the previous op was also a NOP, condense them.
                        lastOp = list.parsedOps[list.parsedOps.length-1];
                        if(lastOp.op != 'NOP') lastOp = null;
                    }
                    if(lastOp) lastOp.length++;
                    else list.parsedOps.push({
                        op:     'NOP',
                        offset: dl.offset - 1,
                        length: 1,
                    });
                    break;
                }
                case 0x08: { //load CP reg
                    const reg = dl.nextU8();
                    const val = dl.nextU32();
                    list.parsedOps.push({
                        op:     'LoadCP',
                        offset: dl.offset - 6,
                        length: 6,
                        params: {reg:reg, val:val},
                    });
                    console.log("SET CP %s to %s",
                        reg.toString(16), val.toString(16));
                    this.gx.cp.setReg(reg, val);
                    break;
                }
                case 0x10: { //load XF reg
                    const start  = dl.offset - 1;
                    const count  = (dl.nextU16() & 0xF) + 1;
                    const offs   = dl.nextU16();
                    const params = [];
                    for(let i=0; i<count; i++) {
                        const val = dl.nextU32();
                        params.push(val);
                        this.gx.xf.setReg(i+offs, val);
                    }
                    list.parsedOps.push({
                        op:     'LoadXF',
                        offset: start,
                        length: dl.offset - start,
                        params: {reg:offs, data:params},
                    });
                    break;
                }
                case 0x20:   //load IndxA (position mtx)
                case 0x28:   //load IndxB (normal mtx)
                case 0x30:   //load IndxC (post mtxs)
                case 0x38: { //load IndxD (lights)
                    const start = dl.offset - 1;
                    const idx   = dl.nextU16();
                    let   offs  = dl.nextU16();
                    const size  = (offs >> 12) + 1;
                    offs = offs & 0x0FFF;
                    //XXX what to do with these?
                    console.log("SET INDX 0x%s offs 0x%s count %d",
                        ((opcode >> 3) - 4).toString(16), offs.toString(16), size);
                    list.parsedOps.push({
                        op:     'SetIndx',
                        offset: dl.offset - 5,
                        length: 5,
                        params: {idx:idx, offs:offs, size:size},
                    });
                    break;
                }
                case 0x40: { //call dlist
                    //XXX can we do anything useful?
                    let offs = dl.offset - 1;
                    let addr = dl.nextU32();
                    let size = dl.nextU32();
                    console.log("CALL DLIST from %s to %s size %s",
                        offs.toString(16), addr.toString(16), size.toString(16));
                    list.parsedOps.push({
                        op:     'Call',
                        offset: offs,
                        length: 9,
                        params: {addr:addr, size:size},
                    });
                    break;
                }
                case 0x48: { //Invalidate vtx cache
                    //nothing to do
                    list.parsedOps.push({
                        op:     'InvCache',
                        offset: dl.offset - 1,
                        length: 1,
                    });
                    break;
                }
                case 0x61: { //load BP reg
                    //XXX
                    const val = dl.nextU32();
                    console.log("SET BP %s to %s",
                        (val >> 24).toString(16), (val & 0xFFFFFF).toString(16));
                    list.parsedOps.push({
                        op:     'LoadBP',
                        offset: dl.offset - 5,
                        length: 5,
                        params: {reg: val >> 24, val: val & 0xFFFFFF},
                    });
                    break;
                }
                default: {
                    list.parsedOps.push({
                        op:     'Unknown',
                        offset: dl.offset - 1,
                        length: 1,
                        params: {op:opcode},
                    });
                    console.error("Dlist unknown opcode 0x%s at 0x%s",
                        opcode.toString(16),
                        (dl.offset-1).toString(16),
                        hexdump(this.data.dlist.data));
                    //throw new Error("Unknown dlist opcode 0x"+
                    //    opcode.toString(16) + " at 0x"+
                    //    (dl.offset-1).toString(16));
                    return;
                }
            }
        }
    }

    op_draw(opcode) {
        /** Execute draw opcode.
         *  Called from execute().
         */
        const start = this.data.dlist.offset - 1;
        const mode  = (opcode & 0x78) >> 3;
        const vat   = opcode & 7;
        const count = this.data.dlist.nextU16();
        const vtxs  = [];
        for(let i=0; i<count; i++) {
            const vtx = this._nextVertex(vat);
            vtx.index = i;
            vtxs.push(vtx);
            console.log("draw vtx", vtx, this.gx.cp.getState());
        }
        this.curList.parsedOps.push({
            op:     'Draw'+DrawOpNames[mode],
            offset: start,
            length: this.data.dlist.offset - start,
            params: {vat:vat, count:count},
        });
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
                let v = src.nextU16();
                b = ( v        & 0x1F) * (255/31);
                g = ((v >>  5) & 0x3F) * (255/63);
                r = ((v >> 11) & 0x1F) * (255/31);
                break;
            }
            case 1: { //RGB888
                r = src.nextU8();
                g = src.nextU8();
                b = src.nextU8();
                break;
            }
            case 2: { //RGBX8888
                r = src.nextU8();
                g = src.nextU8();
                b = src.nextU8();
                src.nextU8(); //discard
                break;
            }
            case 3: { //RGBA4444
                let v = src.nextU16();
                a = ( v        & 0xF) * (255/15);
                b = ((v >>  4) & 0xF) * (255/15);
                g = ((v >>  8) & 0xF) * (255/15);
                r = ((v >> 12) & 0xF) * (255/15);
                break;
            }
            case 4: { //RGBA6666
                let v = (src.nextU8() << 16) | src.nextU16();
                a = ( v        & 0x3F) * (255/63);
                b = ((v >>  6) & 0x3F) * (255/63);
                g = ((v >> 12) & 0x3F) * (255/63);
                r = ((v >> 18) & 0x3F) * (255/63);
                break;
            }
            case 5: { //RGBA8888
                r = src.nextU8();
                g = src.nextU8();
                b = src.nextU8();
                a = src.nextU8();
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

        const vals = [];
        for(let i=0; i<(count ? 9 : 3); i++) {
            let val = null;
            switch(format) {
                //u8, u16 are not valid for normals
                case 1: val = src.nextS8()  / (1<< 6); break;
                case 3: val = src.nextS16() / (1<<14); break;
                case 4: val = src.nextFloat();     break;
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
        const format = vcd[field+'FMT']  || 0;
        const count  = vcd[field+'CNT']  || 0;

        const vals = [];
        const cntMin = (field.startsWith('TEX') ? 1 : 2);
        for(let i=0; i<count + cntMin; i++) {
            let val = null;
            switch(format) {
                case 0: val = src.nextU8();    break;
                case 1: val = src.nextS8();    break;
                case 2: val = src.nextU16();   break;
                case 3: val = src.nextS16();   break;
                case 4: val = src.nextFloat(); break;
                default: console.error(
                    "Invalid format for attribute %s (fmt=%s count=%s shift=%s)",
                    field, format, count, shift);
            }
            if((format == 0 || format == 1) && vcd.BYTEDEQUANT) val /= (1 << shift);
            else if(format == 2 || format == 3) val /= (1 << shift);
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
        for(const field of this.gx.vatFieldOrder) {
            const fmt = vcd[field];
            if(field.endsWith('IDX')) { //always 8-bit direct
                if(fmt) vtx[field] = dl.nextU8();
                //else, no data
            }
            else switch(fmt) {
                case 0: break; //no data
                case 1: vtx[field] = this._nextValue(field, dl, vcd); break; //direct
                case 2:   //8-bit index
                case 3: { //16-bit index
                    const idx = (fmt == 2 ? dl.nextU8() : dl.nextU16());
                    const src = this.data[field];
                    if(src == null) {
                        //throw new Error("Field is null: " + String(field));
                        vtx[field] = null;
                    }
                    else {
                        if(this.gx.cp.arrayStride[field] == undefined) {
                            console.error("No array stride for field", field);
                            src.seek(idx);
                        }
                        else src.seek(idx * this.gx.cp.arrayStride[field]);
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
                    break;
                default: throw new Error(`BUG: impossible VCD format: ${fmt}`);
            }
        }
        vtx.dataLength = this.data.dlist.offset - vtx.offset;
        if(!vtx.POS) debugger;

        //debug: color by shader ID
        if(this.data._debug) {
            for(const [k,v] of Object.entries(this.data._debug)) vtx[k] = v;
            let c = vtx['COL0'];
            switch(this.data._debug.shaderIdx) {
                case 0: c = [255,   0,   0, 255]; break;
                case 1: c = [  0, 255,   0, 255]; break;
                case 2: c = [  0,   0, 255, 255]; break;
                case 3: c = [255,   0, 255, 255]; break;
                case 4: c = [255, 255,   0, 255]; break;
                case 5: c = [  0, 255, 255, 255]; break;
            }
            //debugColor overrides normal color, but not texture
            //vtx['debugColor'] = c;
        }

        if(!this.didLogVtx) { //debug: log a vertex for examination
            console.log("Vtx", vtx);
            console.log("VCD", vcd);
            console.log("stride", this.gx.cp.arrayStride);
            console.log("data", this.data);
            this.didLogVtx = true;
        }

        //keep stats
        if(this.gx.stats.PNMTXIDX[vtx.PNMTXIDX] == undefined) {
            this.gx.stats.PNMTXIDX[vtx.PNMTXIDX] = 0;
        }
        this.gx.stats.PNMTXIDX[vtx.PNMTXIDX]++;
        return vtx;
    }
}
