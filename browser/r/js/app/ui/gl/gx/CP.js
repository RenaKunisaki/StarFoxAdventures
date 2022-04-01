import {validNumber} from '../GlUtil.js';
import { hex } from '../../../../Util.js';

const NUM_VATS = 8;
export const Reg = {
    MATINDEX_A: 0x30,
    MATINDEX_B: 0x40,
    VCD_LO:     0x50, //0-7
    VCD_HI:     0x60, //0-7
    VAT_A:      0x70, //0-7
    VAT_B:      0x80, //0-7
    VAT_C:      0x90, //0-7
    ARRAY_BASE_VTXS:       0xA0,
    ARRAY_BASE_NORMALS:    0xA1,
    ARRAY_BASE_COLOR:      0xA2, //0-1
    ARRAY_BASE_TEXCOORD:   0xA4, //0-7
    ARRAY_BASE_INDEX:      0xAC, //0-3, general purpose array
    ARRAY_STRIDE_VTXS:     0xB0,
    ARRAY_STRIDE_NORMALS:  0xB1,
    ARRAY_STRIDE_COLOR:    0xB2,
    ARRAY_STRIDE_TEXCOORD: 0xB4,
    ARRAY_STRIDE_INDEX:    0xBC,
};
export const Format = { //vertex attribute data
    Coord: {u8:0, s8:1, u16:2, s16:3, float:4},
    Normal:{s8:1, s16:3, float:4},
    Color: {rgb565:0, rgb888:1, rgbx8888:2, rgba4444:3, rgba6666:4, rgba8888:5},
};

const VAT_MASK = { //[mask, shift]
    //VAT A
    POSCNT:        [0x01,  0],
    POSFMT:        [0x07,  1],
    POSSHFT:       [0x1F,  4],
    NRMCNT:        [0x01,  9],
    NRMFMT:        [0x07, 10],
    COL0CNT:       [0x01, 13],
    COL0FMT:       [0x07, 14],
    COL1CNT:       [0x01, 17],
    COL1FMT:       [0x07, 18],
    TEX0CNT:       [0x01, 21],
    TEX0FMT:       [0x07, 22],
    TEX0SHFT:      [0x1F, 25],
    BYTEDEQUANT:   [0x01, 30],
    NORMALINDEX3:  [0x01, 31],

    //VAT B
    TEX1CNT:       [0x01,  0],
    TEX1FMT:       [0x07,  1],
    TEX1SHFT:      [0x1F,  4],
    TEX2CNT:       [0x01,  9],
    TEX2FMT:       [0x07, 10],
    TEX2SHFT:      [0x1F, 13],
    TEX3CNT:       [0x01, 18],
    TEX3FMT:       [0x07, 19],
    TEX3SHFT:      [0x1F, 22],
    TEX4CNT:       [0x01, 27],
    TEX4FMT:       [0x07, 28],
    VCACHE_ENHANCE:[0x01, 31], //"must always be 1"

    //VAT C
    TEX4SHFT:      [0x1F,  0],
    TEX5CNT:       [0x01,  5],
    TEX5FMT:       [0x07,  6],
    TEX5SHFT:      [0x1F,  9],
    TEX6CNT:       [0x01, 14],
    TEX6FMT:       [0x07, 15],
    TEX6SHFT:      [0x1F, 18],
    TEX7CNT:       [0x01, 23],
    TEX7FMT:       [0x07, 24],
    TEX7SHFT:      [0x1F, 27],
};

export default class CP {
    /** Command Processor subsystem for GX.
     */
    constructor(gx) {
        this.gx = gx;
        this._rawVals = {};
        this.reset();
    }

    reset() {
        /** Reset all state back to default.
         */
        this.vcd  = [];
        this.arrayBase = {
            POS: 0, NRM: 0, COL0:0, COL1:0, TEX0:0, TEX1:0, TEX2:0, TEX3:0,
            TEX4:0, TEX5:0, TEX6:0, TEX7:0, IDXA:0, IDXB:0, IDXC:0, IDXD:0,
        };
        this.arrayStride = {
            POS: 0, NRM: 0, COL0:0, COL1:0, TEX0:0, TEX1:0, TEX2:0, TEX3:0,
            TEX4:0, TEX5:0, TEX6:0, TEX7:0, IDXA:0, IDXB:0, IDXC:0, IDXD:0,
        };
        for(let i=0; i<NUM_VATS; i++) {
            this.vcd.push({});
            this.setReg(i+Reg.VCD_LO, 0x200);
            this.setReg(i+Reg.VCD_HI, 0);
            this.setReg(i+Reg.VAT_A, 0x40000000);
            this.setReg(i+Reg.VAT_B, 0x80000000);
            this.setReg(i+Reg.VAT_C, 0);
        }
        //console.log("CP reset OK");
    }

    getReg(reg) {
        return this._rawVals[reg];
    }

    setReg(reg, val) {
        /** Set CP register.
         *  @param {int} reg Register ID.
         *  @param {int} val Value, which should be a 32-bit integer.
         */
        validNumber(reg, val);
        //console.log(`CP SET REG 0x${hex(reg,2)} = 0x${hex(val,8)}`);
        this._rawVals[reg] = val;
        switch(reg) {
            //0x30: MATINDEX_A - Texture matrix idx 0-3 XXX
            //0x40: MATINDEX_B - Texture matrix idx 4-7
            case 0x50: this._setVcdLo(0, val); break;
            case 0x51: this._setVcdLo(1, val); break;
            case 0x52: this._setVcdLo(2, val); break;
            case 0x53: this._setVcdLo(3, val); break;
            case 0x54: this._setVcdLo(4, val); break;
            case 0x55: this._setVcdLo(5, val); break;
            case 0x56: this._setVcdLo(6, val); break;
            case 0x57: this._setVcdLo(7, val); break;
            case 0x60: this._setVcdHi(0, val); break;
            case 0x61: this._setVcdHi(1, val); break;
            case 0x62: this._setVcdHi(2, val); break;
            case 0x63: this._setVcdHi(3, val); break;
            case 0x64: this._setVcdHi(4, val); break;
            case 0x65: this._setVcdHi(5, val); break;
            case 0x66: this._setVcdHi(6, val); break;
            case 0x67: this._setVcdHi(7, val); break;
            case 0x70: this._setVcdFmtA(0, val); break;
            case 0x71: this._setVcdFmtA(1, val); break;
            case 0x72: this._setVcdFmtA(2, val); break;
            case 0x73: this._setVcdFmtA(3, val); break;
            case 0x74: this._setVcdFmtA(4, val); break;
            case 0x75: this._setVcdFmtA(5, val); break;
            case 0x76: this._setVcdFmtA(6, val); break;
            case 0x77: this._setVcdFmtA(7, val); break;
            case 0x80: this._setVcdFmtB(0, val); break;
            case 0x81: this._setVcdFmtB(1, val); break;
            case 0x82: this._setVcdFmtB(2, val); break;
            case 0x83: this._setVcdFmtB(3, val); break;
            case 0x84: this._setVcdFmtB(4, val); break;
            case 0x85: this._setVcdFmtB(5, val); break;
            case 0x86: this._setVcdFmtB(6, val); break;
            case 0x87: this._setVcdFmtB(7, val); break;
            case 0x90: this._setVcdFmtC(0, val); break;
            case 0x91: this._setVcdFmtC(1, val); break;
            case 0x92: this._setVcdFmtC(2, val); break;
            case 0x93: this._setVcdFmtC(3, val); break;
            case 0x94: this._setVcdFmtC(4, val); break;
            case 0x95: this._setVcdFmtC(5, val); break;
            case 0x96: this._setVcdFmtC(6, val); break;
            case 0x97: this._setVcdFmtC(7, val); break;
            case 0xA0: this._setArrayBase('POS',  val); break;
            case 0xA1: this._setArrayBase('NRM',  val); break;
            case 0xA2: this._setArrayBase('COL0', val); break;
            case 0xA3: this._setArrayBase('COL1', val); break;
            case 0xA4: this._setArrayBase('TEX0', val); break;
            case 0xA5: this._setArrayBase('TEX1', val); break;
            case 0xA6: this._setArrayBase('TEX2', val); break;
            case 0xA7: this._setArrayBase('TEX3', val); break;
            case 0xA8: this._setArrayBase('TEX4', val); break;
            case 0xA9: this._setArrayBase('TEX5', val); break;
            case 0xAA: this._setArrayBase('TEX6', val); break;
            case 0xAB: this._setArrayBase('TEX7', val); break;
            case 0xAC: this._setArrayBase('IDXA', val); break;
            case 0xAD: this._setArrayBase('IDXB', val); break;
            case 0xAE: this._setArrayBase('IDXC', val); break;
            case 0xAF: this._setArrayBase('IDXD', val); break;
            case 0xB0: this._setArrayStride('POS',  val); break;
            case 0xB1: this._setArrayStride('NRM',  val); break;
            case 0xB2: this._setArrayStride('COL0', val); break;
            case 0xB3: this._setArrayStride('COL1', val); break;
            case 0xB4: this._setArrayStride('TEX0', val); break;
            case 0xB5: this._setArrayStride('TEX1', val); break;
            case 0xB6: this._setArrayStride('TEX2', val); break;
            case 0xB7: this._setArrayStride('TEX3', val); break;
            case 0xB8: this._setArrayStride('TEX4', val); break;
            case 0xB9: this._setArrayStride('TEX5', val); break;
            case 0xBA: this._setArrayStride('TEX6', val); break;
            case 0xBB: this._setArrayStride('TEX7', val); break;
            case 0xBC: this._setArrayStride('IDXA', val); break;
            case 0xBD: this._setArrayStride('IDXB', val); break;
            case 0xBE: this._setArrayStride('IDXC', val); break;
            case 0xBF: this._setArrayStride('IDXD', val); break;
            default:
                console.log("Don't know what to do with CP reg %s (val %s)",
                    reg.toString(16), val.toString(16));
        }
    }

    _setVcdLo(vcd, val) { // CP registers 0x50 - 0x57
        //these describe the params of draw commands; whether each
        //field has nothing, the direct value, or an index.
        this.vcd[vcd].PNMTXIDX =  val        & 1; //Position/Normal Matrix Index (*1)
        this.vcd[vcd].T0MIDX   = (val >>  1) & 1; //Texture Coordinate 0 Matrix Index
        this.vcd[vcd].T1MIDX   = (val >>  2) & 1;
        this.vcd[vcd].T2MIDX   = (val >>  3) & 1;
        this.vcd[vcd].T3MIDX   = (val >>  4) & 1;
        this.vcd[vcd].T4MIDX   = (val >>  5) & 1;
        this.vcd[vcd].T5MIDX   = (val >>  6) & 1;
        this.vcd[vcd].T6MIDX   = (val >>  7) & 1;
        this.vcd[vcd].T7MIDX   = (val >>  8) & 1;
        this.vcd[vcd].POS      = (val >>  9) & 3; //Position
        this.vcd[vcd].NRM      = (val >> 11) & 3; //Normal or Normal/Binormal/Tangent
        this.vcd[vcd].COL0     = (val >> 13) & 3; //Color0 (Diffused)
        this.vcd[vcd].COL1     = (val >> 15) & 3; //Color1 (Specular)
        /* (*1) position and normal matrices are stored in 2 seperate areas of
         * internal XF memory, but there is a one to one correspondence between
         * normal and position index. If index 'A' is used for the position,
         * then index 'A' needs to be used for the normal as well.
         */
    }
    _setVcdHi(vcd, val) { // CP registers 0x60 - 0x67
        this.vcd[vcd].TEX0   =  val        & 3; //texture coordinate 0
        this.vcd[vcd].TEX1   = (val >>  2) & 3;
        this.vcd[vcd].TEX2   = (val >>  4) & 3;
        this.vcd[vcd].TEX3   = (val >>  6) & 3;
        this.vcd[vcd].TEX4   = (val >>  8) & 3;
        this.vcd[vcd].TEX5   = (val >> 10) & 3;
        this.vcd[vcd].TEX6   = (val >> 12) & 3;
        this.vcd[vcd].TEX7   = (val >> 14) & 3;
    }
    _setVcdFmtA(vcd, val) { // CP registers 0x70 - 0x77
        //these define the format of the attribute data itself.
        //I think this applies to both direct values and arrays.
        this.vcd[vcd].NORMALINDEX3 = (val >> 31) & 1; //1 or 3 idxs per normal
        this.vcd[vcd].BYTEDEQUANT  = (val >> 30) & 1; //shift applies to u8/s8 (should always be 1)
        this.vcd[vcd].TEX0SHFT     = (val >> 25) & 0x1F;
        this.vcd[vcd].TEX0FMT      = (val >> 22) & 7;
        this.vcd[vcd].TEX0CNT      = (val >> 21) & 1;
        this.vcd[vcd].COL1FMT      = (val >> 18) & 7;
        this.vcd[vcd].COL1CNT      = (val >> 17) & 1;
        this.vcd[vcd].COL0FMT      = (val >> 14) & 7;
        this.vcd[vcd].COL0CNT      = (val >> 13) & 1;
        this.vcd[vcd].NRMFMT       = (val >> 10) & 7;
        this.vcd[vcd].NRMCNT       = (val >>  9) & 1;
        this.vcd[vcd].POSSHFT      = (val >>  4) & 0x1F;
        this.vcd[vcd].POSFMT       = (val >>  1) & 7;
        this.vcd[vcd].POSCNT       =  val        & 1;
        /* (*1) when nine-normals are selected in indirect mode, input will be
         * treated as three staggered indices (one per triple biased by
         * components size), into normal table (note: first index internally
         * biased by 0, second by 1, third by 2)
         */
    }
    _setVcdFmtB(vcd, val) { // CP registers 0x80 - 0x87
        this.vcd[vcd].VCACHE_ENHANCE = (val >> 31) & 1; //must be 1
        this.vcd[vcd].TEX4FMT        = (val >> 28) & 7;
        this.vcd[vcd].TEX4CNT        = (val >> 27) & 1;
        this.vcd[vcd].TEX3SHFT       = (val >> 22) & 0x1F;
        this.vcd[vcd].TEX3FMT        = (val >> 19) & 7;
        this.vcd[vcd].TEX3CNT        = (val >> 18) & 1;
        this.vcd[vcd].TEX2SHFT       = (val >> 13) & 0x1F;
        this.vcd[vcd].TEX2FMT        = (val >> 10) & 7;
        this.vcd[vcd].TEX2CNT        = (val >>  9) & 1;
        this.vcd[vcd].TEX1SHFT       = (val >>  4) & 0x1F;
        this.vcd[vcd].TEX1FMT        = (val >>  1) & 7;
        this.vcd[vcd].TEX1CNT        =  val        & 1;
    }
    _setVcdFmtC(vcd, val) { // CP registers 0x90 - 0x97
        this.vcd[vcd].TEX7SHFT = (val >> 27) & 0x1F;
        this.vcd[vcd].TEX7FMT  = (val >> 24) & 7;
        this.vcd[vcd].TEX7CNT  = (val >> 23) & 1;
        this.vcd[vcd].TEX6SHFT = (val >> 18) & 0x1F;
        this.vcd[vcd].TEX6FMT  = (val >> 15) & 7;
        this.vcd[vcd].TEX6CNT  = (val >> 14) & 1;
        this.vcd[vcd].TEX5SHFT = (val >>  9) & 0x1F;
        this.vcd[vcd].TEX5FMT  = (val >>  6) & 7;
        this.vcd[vcd].TEX5CNT  = (val >>  5) & 1;
        this.vcd[vcd].TEX4SHFT =  val        & 0x1F;
    }
    _setArrayBase(field, val) { // CP registers 0xA0 - 0xAF
        this.arrayBase[field] = val;
    }
    _setArrayStride(field, val) { // CP registers 0xB0 - 0xBF
        //not really used, but included for completeness' sake.
        this.arrayStride[field] = val;
    }

    setVatFormat(vat, params) {
        /** Convenience wrapper for setting up VAT */
        const fields = {
            A: ['POSCNT','POSFMT','POSSHFT','NRMCNT','NRMFMT','COL0CNT',
                'COL0FMT','COL1CNT','COL1FMT','TEX0CNT','TEX0FMT','TEX0SHFT',
                'BYTEDEQUANT','NORMALINDEX3'],
            B: ['TEX1CNT','TEX1FMT','TEX1SHFT','TEX2CNT','TEX2FMT','TEX2SHFT',
                'TEX3CNT','TEX3FMT','TEX3SHFT','TEX4CNT','TEX4FMT',
                'VCACHE_ENHANCE'],
            C: ['TEX4SHFT','TEX5CNT','TEX5FMT','TEX5SHFT','TEX6CNT','TEX6FMT',
                'TEX6SHFT','TEX7CNT','TEX7FMT','TEX7SHFT'],
        }
        let regs = {
            A: this.getReg(vat+0x70),
            B: this.getReg(vat+0x80),
            C: this.getReg(vat+0x90),
        };
        for(let r of ['A', 'B', 'C']) {
            for(const name of fields[r]) {
                let v = regs[r];
                if(params[name] != undefined) {
                    const [mask, shift] = VAT_MASK[name];
                    v = (v & ~(mask << shift)) | (params[name] << shift);
                }
                regs[r] = v;
            }
        }
        this.setReg(vat+0x70, regs.A);
        this.setReg(vat+0x80, regs.B);
        this.setReg(vat+0x90, regs.C);
    }

    getState() {
        /** Return dict of state info for debug. */
        const r_MATINDEX_A = this.getReg(Reg.MATINDEX_A);
        const r_MATINDEX_B = this.getReg(Reg.MATINDEX_B);
        const descrNames = ['-', 'DIR', 'i8', 'i16'];

        const result = {
            vat:[],
            MATINDEX_A: {
                TEX3IDX: (r_MATINDEX_A >> 24) & 0x3F,
                TEX2IDX: (r_MATINDEX_A >> 18) & 0x3F,
                TEX1IDX: (r_MATINDEX_A >> 12) & 0x3F,
                TEX0IDX: (r_MATINDEX_A >>  6) & 0x3F,
                POSIDX:   r_MATINDEX_A        & 0x3F,
                _value:  hex(r_MATINDEX_A),
            },
            MATINDEX_B: {
                TEX7IDX: (r_MATINDEX_B >> 18) & 0x3F,
                TEX6IDX: (r_MATINDEX_B >> 12) & 0x3F,
                TEX5IDX: (r_MATINDEX_B >>  6) & 0x3F,
                TEX4IDX:  r_MATINDEX_B        & 0x3F,
                _value:  hex(r_MATINDEX_B),
            },
        };
        for(let i=0; i<NUM_VATS; i++) {
            const vcd_lo = this.getReg(Reg.VCD_LO + i);
            const vcd_hi = this.getReg(Reg.VCD_HI + i);
            const vatA   = this.getReg(Reg.VAT_A  + i);
            const vatB   = this.getReg(Reg.VAT_B  + i);
            const vatC   = this.getReg(Reg.VAT_C  + i);
            result.vat.push({
                VCD_LO: {
                    COL1:   descrNames[(vcd_lo >> 15) & 3], //specular
                    COL0:   descrNames[(vcd_lo >> 13) & 3], //diffuse
                    NRM:    descrNames[(vcd_lo >> 11) & 3],
                    POS:    descrNames[(vcd_lo >>  9) & 3],
                    T7MIDX: descrNames[(vcd_lo >>  8) & 1],
                    T6MIDX: descrNames[(vcd_lo >>  7) & 1],
                    T5MIDX: descrNames[(vcd_lo >>  6) & 1],
                    T4MIDX: descrNames[(vcd_lo >>  5) & 1],
                    T3MIDX: descrNames[(vcd_lo >>  4) & 1],
                    T2MIDX: descrNames[(vcd_lo >>  3) & 1],
                    T1MIDX: descrNames[(vcd_lo >>  2) & 1],
                    T0MIDX: descrNames[(vcd_lo >>  1) & 1], //texcoord mtx
                    PMIDX:  descrNames[ vcd_lo        & 1], //position/normal mtx
                    _value: hex(vcd_lo),
                },
                VCD_HI: { //texture coords
                    TEX7: descrNames[(vcd_hi >> 14) & 3],
                    TEX6: descrNames[(vcd_hi >> 12) & 3],
                    TEX5: descrNames[(vcd_hi >> 10) & 3],
                    TEX4: descrNames[(vcd_hi >>  8) & 3],
                    TEX3: descrNames[(vcd_hi >>  6) & 3],
                    TEX2: descrNames[(vcd_hi >>  4) & 3],
                    TEX1: descrNames[(vcd_hi >>  2) & 3],
                    TEX0: descrNames[ vcd_hi        & 3],
                    _value: hex(vcd_hi),
                },
                VAT_A: {
                    NORMALS:    ((vatA >> 31) & 0x01) ? 1 : 3,
                    BYTEDEQUANT: (vatA >> 30) & 0x01,
                    TEX0SHFT:    (vatA >> 25) & 0x1F,
                    TEX0FMT:     (vatA >> 22) & 0x07,
                    TEX0CNT:     (vatA >> 21) & 0x01,
                    COL1FMT:     (vatA >> 18) & 0x07,
                    COL1CNT:     (vatA >> 17) & 0x01,
                    COL0FMT:     (vatA >> 14) & 0x07,
                    COL0CNT:     (vatA >> 13) & 0x01,
                    NRMFMT:      (vatA >> 10) & 0x07,
                    NRMCNT:      (vatA >>  9) & 0x01,
                    POSSHFT:     (vatA >>  4) & 0x1F,
                    POSFMT:      (vatA >>  1) & 0x07,
                    POSCNT:       vatA        & 0x01,
                    _value: hex(vatA),
                },
                VAT_B: {
                    VCACHE_ENHANCE: (vatB >> 31) & 0x01,
                    TEX4FMT:        (vatB >> 28) & 0x07,
                    TEX4CNT:        (vatB >> 27) & 0x01,
                    TEX3SHFT:       (vatB >> 22) & 0x1F,
                    TEX3FMT:        (vatB >> 19) & 0x07,
                    TEX3CNT:        (vatB >> 18) & 0x01,
                    TEX2SHFT:       (vatB >> 13) & 0x1F,
                    TEX2FMT:        (vatB >> 10) & 0x07,
                    TEX2CNT:        (vatB >>  9) & 0x01,
                    TEX1SHFT:       (vatB >>  4) & 0x1F,
                    TEX1FMT:        (vatB >>  1) & 0x07,
                    TEX1CNT:         vatB        & 0x01,
                    _value: hex(vatB),
                },
                VAT_C: {
                    TEX7SHFT:       (vatC >> 27) & 0x1F,
                    TEX7FMT:        (vatC >> 24) & 0x07,
                    TEX7CNT:        (vatC >> 23) & 0x01,
                    TEX6SHFT:       (vatC >> 18) & 0x1F,
                    TEX6FMT:        (vatC >> 15) & 0x07,
                    TEX6CNT:        (vatC >> 14) & 0x01,
                    TEX5SHFT:       (vatC >>  9) & 0x1F,
                    TEX5FMT:        (vatC >>  6) & 0x07,
                    TEX5CNT:        (vatC >>  5) & 0x01,
                    TEX4SHFT:        vatC        & 0x1F,
                    _value: hex(vatC),
                },
            });
        }
        return result;
    }
}
