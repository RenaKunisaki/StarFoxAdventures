import {validNumber} from '/js/Util.js';

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
        for(let i=0; i<8; i++) {
            this.vcd.push({});
            this._setVcdLo(i, 0);
            this._setVcdHi(i, 0);
            this._setVcdFmtA(i, 0x40000000);
            this._setVcdFmtB(i, 0x80000000);
            this._setVcdFmtC(i, 0);
        }
    }

    getReg(reg) {
        return this._rawVals[reg];
    }

    setReg(reg, val) {
        /** Set CP register.
         *  reg: Register ID.
         *  val: Value, which should be a 32-bit integer.
         */
        validNumber(reg, val);
        this._rawVals[reg] = val;
        switch(reg) {
            //0x30: MAXINDEX_A - Texture matrix idx 0-3 XXX
            //0x40: MAXINDEX_B - Texture matrix idx 4-7
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
        //these define the format of the field within the display list.
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
        //these define the format of the attribute data
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
}
